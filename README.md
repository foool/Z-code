Z codes
=======

### 1. Introduction
Z codes are a class of repair-bandwidth-efficient erasure codes that can achieve the optimal repair bandwidth for a single data node failure with only XOR operations. This demo library provides interfaces to generate PGM(Parity Generator Matrix) of Z codes, perform in-memory encoding and repairing. Free to enjoy it.

### 2. Files' description
> - common.c provides some common functions that would be used in other files
> - mat.c performs matrix operations
> - mcoding.c performs in-memory coding by computing matrix product of the PGM and data blocks
> - region_xor.c performs XORing two aligned regions
> - zcode.c provides several interfaces(initialization, encoding, repairing and deconstruction) of Z codes

### 3. **Note**
To constrain the PGM in a reasonable size, parameters of Z codes must meet the restriction:$m = 2,3,4$ and when $m = 2 , k < 14$, $m = 3 , k < 10$ and $m = 4 , k < 6$. Since this is a **DEMO** library, please don't adopt it in a production environment directly.  
