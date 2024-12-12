# A RISC-V Simulator for the Tensor Processing Unit

Michelle Zimmermann, Kate Unger, Tanay Biradar

Final Project, CS254 (Advanced Computer Architecture) at UC Santa Barbara

## Abstract
The rapid adoption of machine learning (ML)—particularly deep learning (DL)—methods over the past 15 years has led to a high demand for custom hardware accelerators. Around 2017, Google released a description of the [Tensor Processing Unit](https://arxiv.org/pdf/1704.04760) (TPU), a proprietary ML accelerator with its own instruction set architecture (ISA) that supports around a dozen instructions. We are interested in running arbitrary C programs on this accelerator, as “there is a tremendous potential to reduce the amount of time and energy required to complete an operation if we are able to broaden the functionality of the accelerators” ([source](https://engineering.ucsb.edu/news/investing-social-impact)). Previous work, a compiler named the Mullifier (credit to Andrew Alex, UCSB ArchLab - citation needed), has been able to compile a subset of C to the TPU instruction set. However, that subset of C is extremely limited. We contribute a new ISA simulator, written in the Mullifier’s supported subset of C, that can read arbitrary RISC-V assembly and simulate its execution. By doing so, our method allows us to run any arbitrary program compiled to RISC-V assembly on OpenTPU.

## Running the Simulator
The simulator is written in C. Ensure that you have `g++` installed.

1. Write your RISC-V program (see examples below)
2. Convert each instruction to decimal with [this tool](https://luplab.gitlab.io/rvcodecjs/#q=lw+x3,+0(x2)&abi=false&isa=AUTO)
3. Replace the `instructions` array in the `main` method in `main.c` with your decimal program
4. You may choose to log relevant registers and memory addresses to ensure correct output. NOTE that this will not work when running the simulator on OpenTPU with the Mullifier
5. `make && ./simulator`

## Caveats
1. We do not simulate an operating system calling convention. `ecall` simply exits the program for now.
2. Reminder: branch target PCs are *offsets* of the current PC, not absolute
3. PC values are not byte-addressed in our simulator. They are simply indices in the instruction buffer array.

To date, we support the following RISC-V instructions: `add, sub, addi, sll, srl, lw, sw, jalr, beq, bneq, blt, bge, ecall`.

Although these instructions have been successfully implemented and tested in sample programs, more extensive testing is necessary to validate the correctness of our implementation. Finally, our simulator has not yet been fully integrated into the Mullifier. Although our simulator is written strictly in the subset of C that Mullifier supports, we have not yet tested integration with the Mullifier; more work is necessary to ensure that we can indeed run our sample programs on the TPU.


## Example RISC-V Programs

### Basic Arithmetic and Exit
```
addi x6, x6, 5
addi x5, x6, 3
ecall
```

Expected State (Relevant Fields):
```
x6: 5
x5: 9
```


### Load/Store
```
addi x1, x1, 5
addi x4, x4, 6
add x1, x1, x1
addi x2, x2, 420
sw x1, 0(x2)
lw x3, 0(x2)
ecall
```

Expected State (Relevant Fields):
```
x1: 10
x2: 420
x3: 10
x4: 6

mem addr 420: 10
```

### Branch Testing
```
addi x1, x1, 5
addi x2, x2, 5
addi x3, x3, 1
sw x1, 64(x3)
lw x4, 64(x3)
beq x4, x2, 2
ecall	# shouldn’t exit
addi x4, x4, 100
ecall
```

Expected State (Relevant Fields):
```
x1: 5
x2: 5
x3: 1
x4: 5

mem addr 65: 5
```
