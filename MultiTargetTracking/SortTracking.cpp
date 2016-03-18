#include "SortTracking.hpp"

Sort::Sort( int age, int hits)
{
    max_age = age;
    min_hits = hits;
    frame_count = 0;
    trackers.clear();
}

std::vector<data> Sort::Update(std::vector<data>& dets)
{
    frame_count +=1;

    /*First, get the predicted locations form the exsiting trackers*/
    std::vector<data> pred_trks; //store the predict positions
    std::vector<int> to_del; //record the nan index
    data foo;
    for (unsigned int i = 0; i<trackers.size(); i++)
    {
        std::vector<float> pred_box = trackers[i].Predict();
        bool nan_state = false;
        for(unsigned int j=0; j<pred_box.size(); j++)
        {
            if(std::isnan(pred_box[j]) || std::isinf(pred_box[j]))
                nan_state = true;
        }
        if(nan_state)
            to_del.push_back(i);
        else
        {
            foo.bbox = pred_box;
            foo.score = 0;
            foo.index = 0;
            pred_trks.push_back(foo);
        }
    }
    for (int i = to_del.size() - 1; i >=0; i--) //remove the value in reversed 
    {
        std::vector<KalmanBoxTracker>::iterator iter = trackers.begin() + i;
        trackers.erase(iter);
    }
    
    Munkres m; //Hungarian
    m.diag(false);
    get_matched GM = m.assign(dets, pred_trks);  //Hungarian algorithm
    
    /*Then, update matched trackers with assigned detections*/
    for ( int i=0; i < (int)trackers.size(); i++)
    {
        bool notin = true;
        for (unsigned int j = 0; j < GM.unmatched_trks.size(); j++ )
        {
            if(i == GM.unmatched_trks[j])
            {
                notin = false;
                break;
            }
        }

        if(notin)
        {
            for (unsigned int k = 0 ; k<GM.matched.size(); k++)
            {
                if(i == GM.matched[k].second)
                {
                    int idx = GM.matched[k].first;
                    trackers[i].Update(dets[idx].bbox);
                    break;
                }
            }
        }

    }

    /*Next, create and initialise new trackers for unmatched detections
    We relie on the detection accuracy heavily*/
    for (unsigned int i = 0; i < GM.unmatched_dets.size(); i++)
    {
        KalmanBoxTracker kf(dets[i].bbox);
        trackers.push_back(kf);
    }
    int num_tracker = trackers.size();
    
    for (int i = num_tracker-1; i >=0; i--)
    {
        std::vector<float> box = trackers[i].GetState();
        if((trackers[i].time_since_update < 1) & (trackers[i].hit_streak >= min_hits || frame_count <= min_hits))
        {
            data foo;
            foo.bbox = box;
            foo.score = 0;
            foo.index = trackers[i].id + 1;
            remain.push_back(foo);
        }
        num_tracker -=1;
        //remove the dead tracklet
        if(trackers[i].time_since_update > max_age)
        {
            std::vector<KalmanBoxTracker>::iterator iter = trackers.begin() + num_tracker;
            trackers.erase(iter);
        }
    }
    if(remain.size() > 0)
        return remain;
    else
    {
       std::cout<<"There is no valid remaining trackers!"<<std::endl;

       data foo;
       std::vector<float> aa(4,0.0);

       foo.bbox = aa;
       foo.score = 0;
       foo.index = 0;
       remain.push_back(foo);
       return  remain;
    }


}

