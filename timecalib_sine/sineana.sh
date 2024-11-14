#!/bin/bash

root -l -b -q "timingtree.cc"
wait
hadd -f timingtree.root timingtree_ch*
rm timingtree_ch*
root -l -b -q "./process.cc"
echo "All processes are done"

