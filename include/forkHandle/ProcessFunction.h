//
// Created by adiog on 12/18/16.
//

#ifndef CPP_THREAD_PROCESSFUNCTION_H
#define CPP_THREAD_PROCESSFUNCTION_H

using ProcessFunction = std::function<int(FileDescriptorInputReaderStream,
                                          FileDescriptorOutputWriterStream)>;

#endif //CPP_THREAD_PROCESSFUNCTION_H
