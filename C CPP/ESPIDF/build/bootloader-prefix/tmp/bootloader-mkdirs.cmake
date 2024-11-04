# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/ESP_IDF/Espressif/frameworks/esp-idf-v5.3.1/components/bootloader/subproject"
  "D:/HowToWriteCode/LibraryDevices/C CPP/ESPIDF/build/bootloader"
  "D:/HowToWriteCode/LibraryDevices/C CPP/ESPIDF/build/bootloader-prefix"
  "D:/HowToWriteCode/LibraryDevices/C CPP/ESPIDF/build/bootloader-prefix/tmp"
  "D:/HowToWriteCode/LibraryDevices/C CPP/ESPIDF/build/bootloader-prefix/src/bootloader-stamp"
  "D:/HowToWriteCode/LibraryDevices/C CPP/ESPIDF/build/bootloader-prefix/src"
  "D:/HowToWriteCode/LibraryDevices/C CPP/ESPIDF/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/HowToWriteCode/LibraryDevices/C CPP/ESPIDF/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/HowToWriteCode/LibraryDevices/C CPP/ESPIDF/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
