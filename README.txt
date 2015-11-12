###########################################################
#	OPERATING SYSTEMS
# Project: #3
# Date: Mon Oct 26 03:04:28 CDT 2015  
# Student: Lee Nardo (lrn12)
# Files: thread_a.c, thread_b.c
# Summary: This zip contains 2 sources filess, respectively
#  labeled for each part of the project. To compile these
#  files please use the following commands provided.
#
# Commands
#  Compile: gcc -pthread -Wall -g -o thread_a thread_a.c
#  Run: ./thread_a
#
#  A). uses 4 threads to pull resources from the cave.
#   However, there is no locks stopping all pirates from
#   accessing the cave at the same time, thus causing 
#   problems.
#
#  B). uses 4 threads with mutex lock to pull resources
#   from the cave. This allows only one pirate can enter
#   the cave at a time. This way we get accurate results. 
#
###########################################################

