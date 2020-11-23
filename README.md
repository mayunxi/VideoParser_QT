# VideoParser_QT


1. ubuntu18.04 strProfileInfo=string_format("Main");这类使用会出现奔溃

使用core文件调试，报memmove相关信息.
ubuntu16.04没有问题，可能时g++版本问题，ubuntu16.04 g++版本为5.4.0
