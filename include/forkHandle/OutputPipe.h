// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_OUTPUTPIPE_H
#define CPP_THREAD_OUTPUTPIPE_H


class OutputPipe {
public:
    OutputPipe(Pipe &&pipe)
            : fileDescriptorOwner(
            std::make_shared<FileDescriptorOwner>(pipe.passOwnershipOfFileDescriptor(Pipe::Indicator::Write))) {
    }

    std::shared_ptr<FileDescriptorOwner> getFileDescriptorOwner() {
        return fileDescriptorOwner;
    }

private:
    std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner;
};

#endif //CPP_THREAD_OUTPUTPIPE_H
