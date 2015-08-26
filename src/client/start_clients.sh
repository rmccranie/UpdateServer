#!/bin/bash
for ((x=1; x<=10; x++))
{
    echo "Starting client $x "
    ./updateClient $x &
  
}
