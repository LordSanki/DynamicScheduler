echo "N,S,IPC">output.csv
./sim 8 1 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 16 1 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 32 1 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 64 1 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 128 1 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 256 1 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv

./sim 8 2 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 16 2 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 32 2 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 64 2 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 128 2 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 256 2 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv

./sim 8 4 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 16 4 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 32 4 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 64 4 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 128 4 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 256 4 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv

./sim 8 8 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 16 8 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 32 8 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 64 8 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 128 8 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv
./sim 256 8 0 0 0 0 0 traces/val_gcc_trace_mem.txt >> output.csv

echo "">>output.csv
echo "N,S,IPC">>output.csv
./sim 8 1 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 16 1 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 32 1 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 64 1 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 128 1 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 256 1 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv

./sim 8 2 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 16 2 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 32 2 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 64 2 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 128 2 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 256 2 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv

./sim 8 4 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 16 4 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 32 4 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 64 4 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 128 4 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 256 4 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv

./sim 8 8 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 16 8 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 32 8 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 64 8 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 128 8 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv
./sim 256 8 0 0 0 0 0 traces/val_perl_trace_mem.txt >> output.csv

