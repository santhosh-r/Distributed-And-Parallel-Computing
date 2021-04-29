#!/usr/bin/env python3
import os

def run(cmd):
  output = []
  for _ in range(5):
    output.append(int(os.popen(cmd).read()))
  avg = sum(output)/len(output)
  return avg

def main():
  params = [] # matrix size: threads
  with open('run.txt', 'r') as file:
    for line in file:
      params.append(line)
  print('Serial')
  for param in params:
    print('Params: {}\tAverage Time: {}'.format(param, run('./Homework7_Serial.out {} | grep -o \'[0-9]*\''.format(param[:-1]))))
  print('Distributed')
  for param in params:
    print('Params: {}\tAverage Time: {}'.format(param, run('mpirun ./Homework7_Distributed.out {} | grep -o \'[0-9]*\''.format(param[:-1]))))

if __name__ == '__main__':
  main()