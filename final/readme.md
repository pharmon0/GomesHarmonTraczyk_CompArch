In this this tick a value is read into the cache changing the value from Invalid to Exclusive meaning the value is only held within this cache
=============================================================================================
core_0 | processing operation number 61, read at address 00000000000000010111101101100001
core_0 | Incomplete Request (Number 61) on tick 7128. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
core_1 | processing operation number 61, read at address 00000000000000101000010011111000
Block::set_mesi|mesi changing from I -> E
core_1 | Incomplete Request (Number 61) on tick 7128. Reason:
	Cache Miss | Miss:Cache Miss resolved. | Bus:
core_2 | processing operation number 60, read at address 00000000000000101000010011111000
core_2 | Incomplete Request (Number 60) on tick 7128. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
core_3 | processing operation number 59, read at address 00000000000000101000010011111000
core_3 | Incomplete Request (Number 59) on tick 7128. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
Applying tick 7128 to bus
	current token holder is cache_1
	Token handoff to cache_2
=============================================================================================
In this following tick a little while later the same 
=============================================================================================
core_0 | processing operation number 61, read at address 00000000000000010111101101100001
core_0 | Incomplete Request (Number 61) on tick 7158. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
core_1 | processing operation number 62, read at address 00000000000001010101010000111110
core_1 | Incomplete Request (Number 62) on tick 7158. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
core_2 | processing operation number 60, read at address 00000000000000101000010011111000
Block::set_mesi|mesi changing from E -> S
core_2 | Incomplete Request (Number 60) on tick 7158. Reason:
	Cache Miss | Miss:Cache Miss resolved. | Bus:
core_3 | processing operation number 59, read at address 00000000000000101000010011111000
core_3 | Incomplete Request (Number 59) on tick 7158. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
Applying tick 7158 to bus
	current token holder is cache_2
	Token handoff to cache_3
==============================================================================================
the next tick the third core attempts to write to memory at the same address
core_0 | processing operation number 61, read at address 00000000000000010111101101100001
core_0 | Incomplete Request (Number 61) on tick 7188. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
core_1 | processing operation number 62, read at address 00000000000001010101010000111110
core_1 | Incomplete Request (Number 62) on tick 7188. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
core_2 | processing operation number 61, read at address 00000000000001101110100110001001
core_2 | Incomplete Request (Number 61) on tick 7188. Reason:
	Cache Miss | Miss:Waiting on the bus | Bus:Member does not control the bus
core_3 | processing operation number 59, read at address 00000000000000101000010011111000
Block::set_mesi|mesi changing from S -> M
core_3 | Incomplete Request (Number 59) on tick 7188. Reason:
	Cache Miss | Miss:Cache Miss resolved. | Bus:
Applying tick 7188 to bus
	current token holder is cache_3
	Token handoff to cache_0