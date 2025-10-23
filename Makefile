TARGET_EXECS = barrier.out passing_objects.out peru_and_bolivia.out

BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LDFLAGS := -lm -lpthread $(LDFLAGS)

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
MKDIR_P ?= mkdir -p

all: $(TARGET_EXECS)

$(TARGET_EXECS): %.out: $(SRC_DIRS)/%.c
	$(MKDIR_P) $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(BUILD_DIR)/$@ $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)
