#include <gtest/gtest.h>
#include <forkHandle/ForkHandle.h>
#include <forkHandle/ForkHandleRunner.h>
#include "forkHandle/FileDescriptorOwner.h"
#include "forkHandle/Pipe.h"
#include "forkHandle/FileDescriptorInputReaderStream.h"
#include "forkHandle/FileDescriptorOutputWriterStream.h"
#include <chrono>
#include <thread>


TEST(ForkPipeTestSuite, FileDescriptorOwner)
{
    {
        auto fileDescriptorOwner = FileDescriptorOwner{13};
        ASSERT_EQ(13, fileDescriptorOwner.get());
    }

    {
        auto fileDescriptorOwner = FileDescriptorOwner{14};
        auto movedFileDescriptorOwner = std::move(fileDescriptorOwner);
        ASSERT_EQ(movedFileDescriptorOwner.get(), 14);
    }

    {
        auto fileDescriptorOwner = FileDescriptorOwner{15};
        auto fileDescriptorOwnerMock = FileDescriptorOwner{16};
        fileDescriptorOwnerMock = std::move(fileDescriptorOwner);
        ASSERT_EQ(fileDescriptorOwnerMock.get(), 15);
    }
}

TEST(ForkPipeTestSuite, Pipe)
{
    Pipe pipe();

}


int unsafe_process(FileDescriptorInputReaderStream inputReaderStream, FileDescriptorOutputWriterStream outputWriterStream) {
    char roundrobin = '_';

    inputReaderStream >> roundrobin;

    std::cout << "Child: read " << roundrobin << std::endl;

    roundrobin = 'b';

    outputWriterStream << roundrobin;

    std::cout << "Child: written " << roundrobin << std::endl;

    inputReaderStream >> roundrobin;

    std::cout << "Child: read " << roundrobin << std::endl;

    roundrobin = 'd';

    outputWriterStream << roundrobin;

    std::cout << "Child: written " << roundrobin << std::endl;

    return 0;
}

TEST(ForkPipeTestSuite, MainTest)
{

    ForkHandle parentProcess = ForkHandleRunner::runForked(unsafe_process);


    char roundrobin = 'a';
    parentProcess.outputWriterStream << roundrobin;
    std::cout << "Parent: written " << roundrobin << std::endl;

    parentProcess.inputReaderStream >> roundrobin;
    std::cout << "Parent: read " << roundrobin;
    ASSERT_EQ(roundrobin, 'b');

    roundrobin = 'c';
    parentProcess.outputWriterStream << roundrobin;
    std::cout << "Parent: written " << roundrobin << std::endl;

    parentProcess.inputReaderStream >> roundrobin;
    std::cout << "Parent: read " << roundrobin;

    ASSERT_EQ(roundrobin, 'd');
}

