//
// Created by adiog on 12/18/16.
//

#ifndef CPP_THREAD_FILEDESCRIPTOROWNER_H
#define CPP_THREAD_FILEDESCRIPTOROWNER_H

class FileDescriptorOwner {
public:
    FileDescriptorOwner(FileDescriptor fileDescriptor)
            : fileDescriptor(fileDescriptor) {
    }

    FileDescriptorOwner(const FileDescriptorOwner &) = delete;

    FileDescriptorOwner &operator=(const FileDescriptorOwner &) = delete;

    template<typename FileDescriptorOwner>
    void swap(FileDescriptorOwner &&lhsFileDescriptorOwner, FileDescriptorOwner &&rhsFileDescriptorOwner) {
        std::swap(lhsFileDescriptorOwner.fileDescriptor, rhsFileDescriptorOwner.fileDescriptor);
    }

    FileDescriptorOwner(FileDescriptorOwner &&fileDescriptor) {
        swap(*this, fileDescriptor);
    }

    FileDescriptorOwner &operator=(FileDescriptorOwner &&fileDescriptor) {
        swap(*this, fileDescriptor);
        return *this;
    }

    FileDescriptor get() {
        return fileDescriptor;
    }


    ~FileDescriptorOwner() {
        close(fileDescriptor);
    }

private:
    FileDescriptor fileDescriptor;
};

#endif //CPP_THREAD_FILEDESCRIPTOROWNER_H
