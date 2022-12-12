# Copy Service

## Team members:
1. Mura Victor
2. Sabau Eduard
3. Popescu Matei


## Bibliography
1. Subject description:
    https://os.cs.unibuc.ro/so/so-lab-proiect.pdf

## Comand line Interface
```
$ ls
JOB ID  | PROGRESS                                         | STATE
JOB 0   | 0 [                                        ] 100 | WAITING              
JOB 1   | 0 [                                        ] 100 | WAITING              
JOB 2   | 0 [                                        ] 100 | WAITING              
JOB 4   | 0 [######################################  ] 100 | IN_PROGRESS          
JOB 5   | 0 [#################################       ] 100 | IN_PROGRESS          
JOB 6   | 0 [#############################           ] 100 | IN_PROGRESS          
JOB 7   | 0 [############################            ] 100 | IN_PROGRESS          
JOB 8   | 0 [##########################              ] 100 | IN_PROGRESS          
JOB 9   | 0 [#######################                 ] 100 | IN_PROGRESS          
JOB 10  | 0 [######                                  ] 100 | IN_PROGRESS          
JOB 11  | 0 [####                                    ] 100 | IN_PROGRESS          
JOB 12  | 0 [                                        ] 100 | IN_PROGRESS          
JOB 13  | 0 [                                        ] 100 | IN_PROGRESS          
JOB 14  | 0 [                                        ] 100 | WAITING              
JOB 15  | 0 [                                        ] 100 | WAITING              
JOB 16  | 0 [                                        ] 100 | WAITING       

$ cp src_path dst_path
Copy job with ID=X has started!


```

## Compiling and run service
```
make
```


## Testing
#### In Daemon
```
$ cp tests/src/stest.txt dtest.txt
```
#### Unit Tests
```
make test
```