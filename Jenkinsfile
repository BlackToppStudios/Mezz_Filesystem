#!groovy

pipeline {
    agent none
    options {
        buildDiscarder(logRotator(numToKeepStr:'30'))
        timeout(time: 1500, unit: 'SECONDS')
    }
    stages {

        stage('BuildTest') {
            parallel {
                stage('FedoraGcc-Debug') {
                    agent { label "FedoraGcc" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=ON &&
                            ninja &&
                            ./Filesystem_Tester xml &&
                            bash <(curl -s https://codecov.io/bash) -t ${env.CODECOV_TOKEN}
                        """ }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('FedoraGcc-Release') {
                    agent { label "FedoraGcc" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Filesystem_Tester xml
                        """ }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }

                stage('MacOSAir-Debug') {
                    agent { label "MacOSAir" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """#!/bin/bash
                            hostname &&
                            export PATH='$PATH:/usr/local/bin/' &&
                            cmake -E env CXXFLAGS="-fno-var-tracking" cmake -G"Xcode" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=ON &&
                            cmake --build . &&
                           ./Filesystem_Tester xml &&
                           bash <(curl -s https://codecov.io/bash) -t ${env.CODECOV_TOKEN}
                        """ }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('MacOSAir-Release') {
                    agent { label "MacOSAir" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """#!/bin/bash
                            hostname &&
                            export PATH='$PATH:/usr/local/bin/' &&
                            cmake -E env CXXFLAGS="-fno-var-tracking" cmake -G"Xcode" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            cmake --build . &&
                           ./Filesystem_Tester xml
                        """ }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }

                stage('Raspbian-Debug') {
                    agent { label "Raspbian" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=ON &&
                            ninja &&
                            ./Filesystem_Tester xml &&
                            bash <(curl -s https://codecov.io/bash) -t ${env.CODECOV_TOKEN}
                         """ }
                    }
                    post {
                         always {
                             junit "build-debug/**/Mezz*.xml"
                         }
                    }
                }
                stage('Raspbian-Release') {
                    agent { label "Raspbian" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Filesystem_Tester xml
                         """ }
                    }
                    post {
                         always {
                             junit "build-release/**/Mezz*.xml"
                         }
                    }
                }

                stage('UbuntuClang-Debug') {
                    agent { label "UbuntuClang" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env CXXFLAGS="-fno-var-tracking" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=ON &&
                            ninja &&
                            ./Filesystem_Tester xml &&
                            valgrind ./Filesystem_Tester &&
                            bash <(curl -s https://codecov.io/bash) -t ${env.CODECOV_TOKEN}
                         """ }
                     }
                     post {
                         always {
                             junit "build-debug/**/Mezz*.xml"
                         }
                     }
                }
                stage('UbuntuClang-Release') {
                    agent { label "UbuntuClang" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env CXXFLAGS="-fno-var-tracking" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja  &&
                            ./Filesystem_Tester xml &&
                            valgrind ./Filesystem_Tester
                         """ }
                     }
                     post {
                         always {
                             junit "build-release/**/Mezz*.xml"
                         }
                     }
                }

                stage('UbuntuEmscripten-Debug') {
                    agent { label "UbuntuEmscripten" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env EMCC_DEBUG=2 CXXFLAGS="-fno-var-tracking -s WASM=1" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=ON &&
                            ninja &&
                            node Filesystem_Tester.js NoThreads &&
                            bash <(curl -s https://codecov.io/bash) -t ${env.CODECOV_TOKEN}
                        """ }
                    }
                    // Don't capture Emscripten logs, because it cannot make files
                }
                stage('UbuntuEmscripten-Release') {
                    agent { label "UbuntuEmscripten" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env EMCC_DEBUG=2 CXXFLAGS="-fno-var-tracking -s WASM=1" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            node Filesystem_Tester.js NoThreads
                        """ }
                    }
                    // Don't capture Emscripten logs, because it cannot make files
                }

                stage('UbuntuGcc-Debug') {
                    agent { label "UbuntuGcc" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=ON &&
                            ninja &&
                            ./Filesystem_Tester xml &&
                            valgrind ./Filesystem_Tester &&
                            bash <(curl -s https://codecov.io/bash) -t ${env.CODECOV_TOKEN}
                        """ }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('UbuntuGcc-Release') {
                    agent { label "UbuntuGcc" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """#!/bin/bash
                            hostname &&
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Filesystem_Tester xml &&
                            valgrind ./Filesystem_Tester
                        """ }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }

                stage('Windows10Mingw32-Debug') {
                    agent { label "Windows10Mingw32" }
                    steps {
                        bat 'hostname'
                        checkout scm
                        bat 'if not exist "build-debug" mkdir build-debug'
                        dir('build-debug') {
                            bat 'cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=ON'
                            bat 'ninja'
                            bat 'Filesystem_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('Windows10Mingw32-Release') {
                    agent { label "Windows10Mingw32" }
                    steps {
                        bat 'hostname'
                        checkout scm
                        bat 'if not exist "build-release" mkdir build-release'
                        dir('build-release') {
                            bat 'cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF'
                            bat 'ninja'
                            bat 'Filesystem_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }

                stage('Windows10Mingw64-Debug') {
                    agent { label "Windows10Mingw64" }
                    steps {
                        bat 'hostname'
                        checkout scm
                        bat 'if not exist "build-debug" mkdir build-debug'
                        dir('build-debug') {
                            bat 'cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=ON'
                            bat 'ninja'
                            bat 'Filesystem_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('Windows10Mingw64-Release') {
                    agent { label "Windows10Mingw64" }
                    steps {
                        bat 'hostname'
                        checkout scm
                        bat 'if not exist "build-release" mkdir build-release'
                        dir('build-release') {
                            bat 'cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF'
                            bat 'ninja'
                            bat 'Filesystem_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }

                stage('Windows10MSVC-Debug') {
                    agent { label "Windows10MSVC" }
                    steps {
                        bat 'hostname'
                        checkout scm
                        bat 'if not exist "build-debug" mkdir build-debug'
                        dir('build-debug') {
                            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86_amd64 && cmake -G"Visual Studio 16 2019" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF && cmake --build .'
                            bat 'Filesystem_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('Windows10MSVC-Release') {
                    agent { label "Windows10MSVC" }
                    steps {
                        bat 'hostname'
                        checkout scm
                        bat 'if not exist "build-release" mkdir build-release'
                        dir('build-release') {
                            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86_amd64 && cmake -G"Visual Studio 16 2019" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF && cmake --build .'
                            bat 'Filesystem_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }

            }
        } // BuildTest
    } // Stages

}
