#!/usr/bin/env python3
import os

def run(cmd):
  output = []
  for _ in range(5):
    output.append(int(os.popen(cmd).read()))
  avg = sum(output)/len(output)
  return avg, output

def main():
  params = [] # matrix size: processes
  with open('run.txt', 'r') as file:
    for line in file:
      size, process_nums_str = line.split(':')
      size = int(size)
      process_nums = []
      for n in process_nums_str.split(','):
        process_nums.append(int(n))
      params.append([size, process_nums])
  os.chdir('..')
  os.system('make > /dev/null')
  os.system('cp *.out generateInput _misc')
  os.system('make clean > /dev/null')
  os.chdir('_misc')
  for param in params:
    size, process_nums = param
    # if size < 2001:
    #   continue
    os.system('./generateInput {}; mv inputMatrix.csv A.csv'.format(size))
    os.system('./generateInput {}; mv inputMatrix.csv B.csv'.format(size))
    print('M',end='')
    print('Size: {}, '.format(size))
    if size < 2000:
      avg, times = run('./Homework6_Serial.out | grep -o \'[0-9]*\'')
      print('\n\tSerial: {} \t {}'.format(avg, times))
    for n in process_nums:
      avg, times = run('mpirun -n {} ./Homework6_Distributed.out | grep -o \'[0-9]*\''.format(n))
      print('\t{} processes: {} \t {}'.format(n, avg, times))      
    os.system('rm *.csv')
  os.system('rm *.out generateInput')

if __name__ == '__main__':
  main()