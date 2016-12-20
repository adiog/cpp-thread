// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FORKHANDLE_H
#define CPP_THREAD_FORKHANDLE_H

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
        kill(child_pid, SIGTERM);
        int status;
        (void)waitpid(pid, &status, 0);
    }

    const pid_t child_pid;

    FileDescriptorInputReaderStream inputReaderStream;
    FileDescriptorOutputWriterStream outputWriterStream;
};

#endif //CPP_THREAD_FORKHANDLE_H
