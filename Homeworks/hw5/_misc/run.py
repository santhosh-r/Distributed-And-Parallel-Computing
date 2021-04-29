#!/usr/bin/env python3
import os

def run(cmd):
  output = []
  for _ in range(30):
    output.append(int(os.popen(cmd).read()))
  output.sort()
  output = output[:5]
  avg = sum(output)/len(output)
  return output, avg

def main():
  params = [] # matrix size: threads
  with open('run.txt', 'r') as file:
    for line in file:
      size, thread_nums_str = line.split(':')
      size = int(size)
      thread_nums = []
      for n in thread_nums_str.split(','):
        thread_nums.append(int(n))
      params.append([size, thread_nums])
  os.chdir('..')
  os.system('make > /dev/null')
  os.system('cp *.out generateInput _misc')
  os.system('make clean > /dev/null')
  os.chdir('_misc')
  if os.path.isdir('output'):
    os.system('rm -r output')
  os.mkdir('output')
  for param in params:
    size, thread_nums = param
    # if size > 100:
    #   break
    os.system('./generateInput {}; mv inputMatrix.csv A.csv'.format(size))
    os.system('./generateInput {}; mv inputMatrix.csv B.csv'.format(size))
    print('Size: {}, \n\tSerial: {}'.format(size, run('./Homework5_Serial.out | grep -o \'[0-9]*\'')))
    for n in thread_nums:
      print('\t{} thread(s): {}'.format(n, run('./Homework5_Parallel.out {} | grep -o \'[0-9]*\''.format(n))))      
    odir = str(size)
    os.mkdir(odir)
    os.system('mv *.csv {}'.format(odir))
    os.system('mv {} output'.format(odir))
  os.system('rm *.out generateInput')

if __name__ == '__main__':
  main()