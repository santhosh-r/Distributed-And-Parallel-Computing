#!/usr/bin/env python3
import os
import argparse

def run(cmd):
  output = []
  for _ in range(5):
    output.append(int(os.popen(cmd).read()))
  avg = sum(output)/len(output)
  return avg, output

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument("iter", help="no of iterations to run game of life", type=int)
  args = parser.parse_args()
  params = [] # matrix size: threads
  with open('run.txt', 'r') as file:
    for line in file:
      n_str, param = line.split(':')
      nodes = []
      for n in n_str.split(' '):
        nodes.append(int(n))
      params.append([nodes, param[:-1]])
  print('Serial')
  for param in params:
    avg, times = run('./Homework8_Serial.out {} {} | grep -o \'[0-9]*\''.format(param[1], args.iter))
    print('Params: {}\tAverage Time: {} \t {}'.format(param[1], avg, times))
  print('Distributed')
  for param in params:
    for n in param[0]:
      avg, times = run('mpirun -n {} ./Homework8_Distributed.out {} {} | grep -o \'[0-9]*\''.format(n, param[1], args.iter))
      print('Params: {}, n = {}\tAverage Time: {} \t {}'.format(param[1], n, avg, times))

if __name__ == '__main__':
  main()