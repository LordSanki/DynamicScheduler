echo "GCC Trace Regular"
./sim 16 4 0 0 0 0 0 traces/val_gcc_trace_mem.txt > output.txt
diff -iyw --suppress-common-lines output.txt vals/val_1.txt

echo "PERL Trace Regular"
./sim 32 16 0 0 0 0 0 traces/val_perl_trace_mem.txt > output.txt
diff -iyw --suppress-common-lines output.txt vals/val_2.txt

echo "GCC Trace with Cache"
./sim 16 4 32 2048 8 0 0 traces/val_gcc_trace_mem.txt > output.txt
diff -iyw --suppress-common-lines output.txt vals/val_extra_1.txt

echo "PERL Trace with Cache"
./sim 32 8 32 1024 4 2048 8 traces/val_perl_trace_mem.txt > output.txt
diff -iyw --suppress-common-lines output.txt vals/val_extra_2.txt
