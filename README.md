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
$ help
cp -- Start a copy process
cp source_file target_file

ls -- List information about every copy process

quit -- End all processes

cancel -- Cancel an ongoing copy process
cancel target_job

pause -- Halt an ongoing process until resumed
pause target_job

resume -- Resume a paused process
resume target_job

stats -- Get information about an ongoing process
stats target_job
```

```
$ cp src_path dst_path
Copy job with ID=0 has started!
```

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
```

```
$ stats 0
JOB 0   | 0 [############                            ] 100 | IN_PROGRESS
```

```
$ pause 0
Job 0 paused!
```

```
$ resume 0
Job 0 resumed!
```

```
$ cancel 0
Job 0 canceled!
```

```
$ quit 
Quit!
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