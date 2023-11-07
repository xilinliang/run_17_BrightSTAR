#!/bin/bash

let CYCLE=$1

root4star -l -b -q FmsSimRunStarsimAndBfc'($CYCLE, $2)' 
root4star -l -b -q RunFmsJetFinderPro'($CYCLE, $2)' 
