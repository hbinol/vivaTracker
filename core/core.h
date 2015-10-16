/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __core__h
#define __core__h

#include <thread>
#include <functional>

#include "input.h"
#include "listener.h"
#include "output.h"
#include "channel.h"


using namespace std;
using namespace cv;

namespace core {
    
    class thread_guard
    {
        std::thread& t;
    public:
        explicit thread_guard(std::thread& t_): t(t_)
        {}
        ~thread_guard()
        {
            if(t.joinable())
            {
                t.join();
            }
        }
        thread_guard(thread_guard const&)=delete;
        thread_guard& operator=(thread_guard const&)=delete;
    };
    
    /*
     *  Parallel Processing Input and communicating through the channel
     */
    class ProcessInput
    {
    private:
        Ptr<Input> _input;
        Ptr<BufferedImageChannel> _channel;
  
    public:
        ProcessInput(Ptr<Input> &input,
                     Ptr<BufferedImageChannel> &channel):
            _input(input), _channel(channel)
        {}

        void operator()();
    };
    
    
    class ProcessFrame : public MouseListener, public KeyboardListener
    {
    public:
        virtual ~ProcessFrame(){}
        virtual void operator()(size_t frameN, Mat &frame, Mat &output)
        {
            output = frame.clone();
        };
        
        //Inherited from MouseListener. Check MouseListener class for details
        virtual void mouseInput(int event, int x, int y, int flags){};
        virtual void leftButtonDown(int x, int y, int flags){};
        virtual void rightButtonDown(int x, int y, int flags){};
        virtual void middleButtonDown(int x, int y, int flags){};
        virtual void mouseMove(int x, int y, int flags){};
        
        //Inherited from KeyboardListerner. Check KeyboardListener class for details.
        virtual void keyboardInput(int key){};
        
    };
    
    
    
    
    /*
     *  Parallel Processing Output and communicating through the channel
     */
    class ProcessOutput
    {
    private:
        Ptr<Output> _output;
        Ptr<BufferedImageChannel> _channel;

    public:
        ProcessOutput(Ptr<Output> &output,
                      Ptr<BufferedImageChannel> &channel):
            _output(output), _channel(channel)
        {}
   
        void operator()();
    };
    
    
    class Processor
    {
    private:
        Ptr<Input>   _input;
        Ptr<ProcessFrame> _process;
        Ptr<Output>  _output;
        
        
        function<void(size_t frameN, Mat &frame, Mat &output)> _functor;
        
        
        
        string _inputWindowName;
        string _outputWindowName;
        
        bool _showInput;
        bool _showOutput;
        
        bool _mListener;
        bool _kListener;
        
        size_t _inputBufferSize;
        size_t _outputBufferSize;
        
        bool _showTimeInfo;
        
        static void mouseCallback(int event, int x, int y, int flags, void *ptr);
        
    public:
        
        Processor():
        _input(new CameraInput(0, Size(-1,-1))),
        _process(new ProcessFrame()),
        _output(NULL),
        _functor(NULL),
        _inputWindowName("Input"),
        _outputWindowName("Process Output"),
        _showInput(false),
        _showOutput(true),
        _mListener(false),
        _kListener(false),
        _inputBufferSize(10),
        _outputBufferSize(10),
        _showTimeInfo(false)
        {}
        
        Processor(int argc, const char * argv[]) : Processor()
        {
          
        }
        void setInputBufferSize(size_t size)
        {
            _inputBufferSize = size;
        }
        
        void setOutputBufferSize(size_t size)
        {
            _outputBufferSize = size;
        }
        
        void showInput(bool show = true)
        {
            _showInput = show;
        }
        void showTimeInfo(bool show = true)
        {
            _showTimeInfo = show;
        }
        void showOutput(bool show = true)
        {
            _showOutput = show;
        }
        void setInputWindowName(const string &name)
        {
            _inputWindowName = name;
        }
        void setOutputWindowName(const string &name)
        {
            _outputWindowName = name;
        }
        void setInput(Ptr<Input> &input)
        {
            _input = input;
        }
        void setOutput(Ptr<Output> &output)
        {
            _output = output;
        }
        void listenToMouseEvents()
        {
            _mListener = true;
        }
        void listenToKeyboardEvents()
        {
            _kListener = true;
        }
        void setProcess(Ptr<ProcessFrame> &process)
        {
            _process = process;
        }
        
        void setProcess(function<void(size_t frameN, Mat &frame, Mat &output)> functor)
        {
            _functor = functor;
        }
        
        void run();
};
    
    
}

#endif