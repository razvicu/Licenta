#!/bin/bash

licenta_line=`grep -n "load anchor \"licenta/*\"" /etc/pf.conf`
appl_line=`grep -n "load anchor \"com.apple\"" /etc/pf.conf | awk -F : '{print $1}'`
if [[ $licenta_line == '' && $appl_line != '' ]]; then
	sed -i a.bak "$((appl_line+1))i\\
			 anchor \"licenta/*\"\\
			 load anchor \"licenta\" from \"/etc/pf.anchors/licenta\"
				" /etc/pf.conf

fi	
