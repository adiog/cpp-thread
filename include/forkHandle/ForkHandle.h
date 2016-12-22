// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FORKHANDLE_H
#define CPP_THREAD_FORKHANDLE_H

#include <thread>
#include <chrono>
#include <signal.h>
#include "forkHandle/Pipe.h"
#include "forkHandle/InputPipe.h"
#include "forkHandle/OutputPipe.h"
#include "forkHandle/FileDescriptorInputReaderStream.h"
#include "forkHandle/FileDescriptorOutputWriterStream.h"

struct ForkHandle {
    ForkHandle(
            pid_t child_pid,
            Pipe parentToChildPipe,
            Pipe childToParentPipe)

            : child_pid(child_pid),
              inputReaderStream{FileDescriptorInputReaderStream{
                      InputPipe{std::move(childToParentPipe)}.getFileDescriptorOwner()}},
              outputWriterStream{FileDescriptorOutputWriterStream{
                      OutputPipe{std::move(parentToChildPipe)}.getFileDescriptorOwner()}} {
    }

    ~ForkHandle() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000U));
        if (isRunning()) {
            std::cout << "Child running... waiting 1s..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000U));
            kill(child_pid, SIGTERM);
            (void) waitpid(child_pid, &status, 0);
        }
        if ( WIFEXITED(status) ) {
            int exitStatus = WEXITSTATUS(status);
            std::cout << "Child exit with exit status " << exitStatus << std::endl;
        }
    }

    bool isRunning() {
        (void) waitpid(child_pid, &status, WNOHANG);
    }

    const pid_t child_pid;
    int status;

    FileDescriptorInputReaderStream inputReaderStream;
    FileDescriptorOutputWriterStream outputWriterStream;
};

#endif //CPP_THREAD_FORKHANDLE_H
