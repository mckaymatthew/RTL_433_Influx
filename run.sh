#!/bin/bash
rtl_433 -F json | while read line; do echo $line > /dev/udp/10.0.0.23/40007; done;

