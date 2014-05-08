valgrind --leak-check=full --leak-resolution=high --log-file=result ./cash manual.cash > output;
diff -s reference_clean output; 

