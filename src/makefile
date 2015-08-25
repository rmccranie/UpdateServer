SERVER_DIR = server
CLIENT_DIR = client


SUBDIRS = server client 

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

#.PHONY:	server client

#server:
#	$(MAKE) -C $(SERVER_DIR)

#client:
#	$(MAKE) -C $(CLIENT_DIR)

clean:
	$(MAKE) -C $(SERVER_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
