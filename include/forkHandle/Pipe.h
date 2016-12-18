// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_PIPE_H
#define CPP_THREAD_PIPE_H

#include <stdexcept>
#include "FileDescriptor.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


class Pipe {
public:
    enum class Indicator : unsigned int {
        Read = 0U, Write = 1U
    };

    Pipe() {
        if (pipe(inputOutputFileDescriptor) == -1) {
            throw std::runtime_error("");
        }
        hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(Indicator::Read)] = true;
        hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(Indicator::Write)] = true;
    }

    Pipe(const Pipe&) = delete;
    Pipe&operator=(const Pipe&) = delete;

    template<typename Pipe>
    void swap(Pipe &&lhsPipe, Pipe &&rhsPipe)
    {
        std::swap(lhsPipe.hasOwnershipOfFileDescriptorFlag[0], rhsPipe.hasOwnershipOfFileDescriptorFlag[0]);
        std::swap(lhsPipe.inputOutputFileDescriptor[0], rhsPipe.inputOutputFileDescriptor[0]);
        std::swap(lhsPipe.hasOwnershipOfFileDescriptorFlag[1], rhsPipe.hasOwnershipOfFileDescriptorFlag[1]);
        std::swap(lhsPipe.inputOutputFileDescriptor[1], rhsPipe.inputOutputFileDescriptor[1]);
    }

    Pipe(Pipe&& pipe)
    {
        swap(*this, pipe);
    }
    Pipe&operator=(Pipe&& pipe)
    {
        swap(*this, pipe);
        return *this;
    }

    ~Pipe() {
        if (hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(Indicator::Read)]) {
            closePipe(Indicator::Read);
        }
        if (hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(Indicator::Write)]) {
            closePipe(Indicator::Write);
        }
    }

    FileDescriptor passOwnershipOfFileDescriptor(Indicator inputOutputIndicator) {
        hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(inputOutputIndicator)] = false;
        return inputOutputFileDescriptor[static_cast<unsigned int>(inputOutputIndicator)];
    }

private:
    void closePipe(Indicator inputOutputIndicator) {
        close(inputOutputFileDescriptor[static_cast<unsigned int>(inputOutputIndicator)]);
    }

    bool hasOwnershipOfFileDescriptorFlag[2] = {false, false};
    FileDescriptor inputOutputFileDescriptor[2];
};

#endif //CPP_THREAD_PIPE_H
