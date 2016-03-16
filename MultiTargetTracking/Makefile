CC=gcc
CXX=g++
CFLAGS= -m64 -pipe -g -Wall -W -D_REENTRANT -fPIE
CXXFLAGS= -m64 -pipe -g -Wall -W -D_REENTRANT -fPIE
CAFFE_ROOT=/home/samlong/Documents/py-faster-rcnn/caffe-fast-rcnn
INCS=-I$(CAFFE_ROOT)/include -I$(CAFFE_ROOT)/src -I/usr/local/cuda-7.5/include
LIBS=-L$(CAFFE_ROOT)/build/lib/ -L/usr/local/lib/
LDFLAGS= -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio
LDFLAGS +=-lhdf5_hl -lhdf5 -lcaffe -lglog 
SRC:=$(wildcard *.cpp)\
	$(wildcard *.c)
			
OBJ:=$(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SRC))) 


TARGET := MultiPersonTracking
.PHONY : clean all

all: $(TARGET)

$(TARGET):$(OBJ)
	$(CXX) $(CFLAGS) -s -o $@ $^ $(LIBS) $(LDFLAGS)
	cp $@ ../run
%.o : %.cpp
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

%.o : %.c
	${CC} -c ${CFLAGS} ${INCS} $< -o $@

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJ)
