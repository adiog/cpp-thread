// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_INPUTPIPE_H
#define CPP_THREAD_INPUTPIPE_H

#include <memory>
#include "Pipe.h"
#include "FileDescriptorOwner.h"

class InputPipe {
public:
    InputPipe(Pipe &&pipe)
            : fileDescriptorOwner(
            std::make_shared<FileDescriptorOwner>(pipe.passOwnershipOfFileDescriptor(Pipe::Indicator::Read))) {
    }

    std::shared_ptr<FileDescriptorOwner> getFileDescriptorOwner() {
        return fileDescriptorOwner;
    }

private:
    std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner;
};


#endif //CPP_THREAD_INPUTPIPE_H
