# documentation

this is a documentation of all genes and alleles in demo-fsac.

## format

### gene name, `fsGeneID`

gene description

#### allele name, `fsAlleleID`

allele description

## documentation

### species type, `SPECIES_TYPE`

determines the type of the species.

having 2 or more alleles of this gene kills the organism.

#### STA, `ST_A`

the organism is a type A species.

#### STB, `ST_B`

the organism is a type B species.

#### STC, `ST_C`

the organism is a type C species.

### resource dependency, `RESOURCE_DEPENDENCY`

determines the type of resources the organism is dependent on.

#### RDα `RD_ALPHA`

the organism can intake type alpha resource and use it as part of its metabolism.

#### RDβ `RD_BETA`

the organism can intake type beta resource and use it as part of its metabolism.

### reproduction, `REPRODUCTION`

determines the rate at which the population grows.

#### RN, `R_NORMAL`

the population grows at the rate of 1.01 per simulation cycle.

#### R+, `R_PLUS`

the population grows at the rate of 1.05 per simulation cycle.

#### R++, `R_PPLUS`

the population grows at the rate of 1.12 per simulation cycle.
