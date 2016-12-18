#ifndef CPP_THREAD_FORKPIPE_H
#define CPP_THREAD_FORKPIPE_H
#include <chrono>
#include <thread>

#include <shout>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iosfwd>
#include <fstream>
#include <iostream>
#include <libltdl/lt_system.h>
#include <memory>
#include <functional>




struct ParentProcess {
    ParentProcess(
            pid_t child_pid,
            Pipe parentToChildPipe,
            Pipe childToParentPipe)

            : child_pid(child_pid),
              inputReaderStream{FileDescriptorInputReaderStream{
                      InputPipe{std::move(childToParentPipe)}.getFileDescriptorOwner()}},
              outputWriterStream{FileDescriptorOutputWriterStream{
                      OutputPipe{std::move(parentToChildPipe)}.getFileDescriptorOwner()}} {
    }

    ~ParentProcess() {
        kill(child_pid, SIGTERM);
    }

    const pid_t child_pid;

    FileDescriptorInputReaderStream inputReaderStream;
    FileDescriptorOutputWriterStream outputWriterStream;
};

}

#endif //CPP_THREAD_FORKPIPE_H
