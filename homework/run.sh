#!/bin/bash
# Run SimBase.java from lab04/ with homework/<program>
cd ../lab04
javac SimBase.java
java SimBase ../homework/$1
