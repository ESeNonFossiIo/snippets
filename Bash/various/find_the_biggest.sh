#!/bin/bash

ls -lR|awk '{print $5, "\t ",$9 }'|sort -n|tail -1
