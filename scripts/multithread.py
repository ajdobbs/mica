#!/usr/bin/env python

""" Run a program on multiple maus output files on multiple threads """

import sys
import os
import string
import argparse
import subprocess
import math
import threading

def launch_exe(files, namespace):
    """ Worker thread function. Run an executable on a set of MAUS output files """
    print threading.currentThread().getName(), ' starting'
    for i, f in enumerate(files):
        print threading.currentThread().getName(), ' analysing ' + f,
        print ', file ' + str(i+1) + ' of ' + str(len(files))
        # Extract the run number from the filename to set the output file name
        loc, fname = os.path.split(f)
        index = str.find(namespace.input_file, '#')
        if index == -1:
            print 'Could not extract run number from ' + fname + ', skipping'
            continue
        run_number = fname[index:index+namespace.pad]
        ofile = string.replace(namespace.output_file, '#', str(run_number))
        # Call the executable
        subprocess.call([namespace.executable, f, ofile])

    print threading.currentThread().getName(), ' exiting'

def run(namespace):
      """ Loop over input root files, send each file for processing """
      print 'Number of threads to run on: ' + str(namespace.nthreads)
      print 'Executable: ' + namespace.executable
      print 'Input directory: ' + namespace.input_dir
      print 'Input file format: ' + namespace.input_file
      print 'Output file format: ' + namespace.output_file

      # Loop over run numbers and generate file names
      ifilenames = []
      for i in range(namespace.istart, namespace.istop, namespace.istep):
          ifile = string.replace(namespace.input_file, '#', str(i).zfill(namespace.pad))
          ifilenames.append(ifile)
      print 'Number of input files requested: ' + str(len(ifilenames))

      # Search the input directory recursively for files in 
      matches = []
      lfiles = ifilenames[:]
      for root, dirnames, filenames in os.walk(namespace.input_dir):
          print 'Parsing directory: ' + root
          # matches = matches + [os.path.join(root, f) for f in filenames if f in ifilenames]
          for f in filenames:
              if f in lfiles:
                  matches.append(os.path.join(root, f))
                  print 'Found ' + os.path.join(root, f)
                  del lfiles[lfiles.index(f)]
                  # print 'Files left: '
                  # print lfiles
                  if len(lfiles) == 0:
                      break
          if len(lfiles) == 0:
              break

      print 'Number of matched input files found in data directory: ' + str(len(matches))

      # Split the file set found into chunks to distribute to the threads
      nfiles = len(matches)
      chunk_size = int(math.ceil(float(nfiles) / float(namespace.nthreads)))
      chunks = [matches[i:i + chunk_size] for i in xrange(0, len(matches), chunk_size)]
      print 'Chunk size: ' + str(chunk_size)

      # Set up the threads to process the chunks
      threads = []
      for i in range(namespace.nthreads):
          if i > (len(chunks) - 1):
              break
          t = threading.Thread(target=launch_exe, args=(chunks[i], namespace))
          threads.append(t)
          t.start()
      for t in threads:
          t.join()

if __name__ == "__main__" : 
  parser = argparse.ArgumentParser( description='Multithread processing ' +\
                'of MAUS output files' )
  parser.add_argument( '-n', '--nthreads', type=int, default=1, \
                       help='Number of processor threads to use')
  parser.add_argument( '-b', '--istart', type=int, default=0, \
                       help='Starting number of run range')
  parser.add_argument( '-e', '--istop', type=int, default=1, \
                       help='Stop number of run range')
  parser.add_argument( '-i', '--istep', type=int, default=1, \
                       help='Run range increment')
  parser.add_argument( '-x', '--executable', type=str, default='mica', \
                       help='Location of the executable to run')
  parser.add_argument( '-p', '--pad', type=int, default=5, \
                       help='Number of digits in run number format')
  parser.add_argument( '-d', '--input_dir', type=str, default='.', \
                       help='Input directory path')
  parser.add_argument( '-f', '--input_file', type=str, default='#_recon.root', \
                       help='Input file name, # will be substituted for run number')
  parser.add_argument( '-o', '--output_file', type=str, default='analysis_#.pdf', \
                       help='Output file name, # will be substituted for run number')

  try :
    namespace = parser.parse_args()
  except :
    raise
  else :
    run(namespace)

  print "Complete"
