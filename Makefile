all: access_mem create_mem

access_mem: access_mem.cc
	g++ access_mem.cc -o access_mem

manage_mem: manage_mem.cc
	g++ manage_mem.cc -o manage_mem
