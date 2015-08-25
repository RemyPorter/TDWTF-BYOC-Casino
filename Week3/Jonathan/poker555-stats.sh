#!/bin/bash

yes "" | ./poker555 | head -n 100000001 > poker555-sample.txt

echo -n "Royal Flush: "
fgrep -i royal poker555-sample.txt | wc -l

echo -n "Straight Flush: "
fgrep -i "straight flush" poker555-sample.txt | wc -l

echo -n "Five of a Kind: "
fgrep -i five poker555-sample.txt | wc -l

echo -n "Four of a Kind: "
fgrep -i four poker555-sample.txt | wc -l

echo -n "Full House: "
fgrep -i full poker555-sample.txt | wc -l

echo -n "Flush: "
fgrep -i flush poker555-sample.txt | fgrep -v -i straight | fgrep -v -i royal | wc -l

echo -n "Straight: "
fgrep -i straight poker555-sample.txt | fgrep -v -i flush | wc -l

echo -n "Three of a Kind: "
fgrep -i three poker555-sample.txt | wc -l

echo -n "Two Pair: "
fgrep -i "two pair" poker555-sample.txt | wc -l

echo -n "Pair: "
fgrep -i pair poker555-sample.txt | fgrep -v -i two | wc -l

echo -n "High Card: "
fgrep -i high poker555-sample.txt | fgrep -v -i straight | wc -l
