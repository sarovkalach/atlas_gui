#!/bin/bash

RESULT=$(ssh atlas3d@85.143.2.188 'test -f '$1' ;  echo $?')

echo $RESULT
