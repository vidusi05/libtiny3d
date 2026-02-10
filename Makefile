# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude

# Directories
SRC_DIR = src
DEMO_DIR = demo
BUILD_DIR = build
TESTS_DIR = tests
PROJECT_DIR = $(shell pwd)/..

# Executable names
SOCCER_EXE = soccer_ball
CUBE_EXE = test_math
VISUAL_EXE = test_visual

# Source files
SOCCER_SRC = $(DEMO_DIR)/main_soccerball.c $(SRC_DIR)/canvas.c $(SRC_DIR)/math3d.c $(SRC_DIR)/renderer.c $(SRC_DIR)/lighting.c $(SRC_DIR)/animation.c
CUBE_SRC = $(TESTS_DIR)/test_math.c $(SRC_DIR)/math3d.c $(SRC_DIR)/canvas.c
VISUAL_SRC = $(TESTS_DIR)/test_visual.c $(SRC_DIR)/canvas.c $(SRC_DIR)/math3d.c $(SRC_DIR)/renderer.c $(SRC_DIR)/lighting.c $(SRC_DIR)/animation.c

# Targets
.PHONY: all image cube_video soccerball_video visual_video videos clean

# Default build
all: $(BUILD_DIR)/$(SOCCER_EXE) $(BUILD_DIR)/$(CUBE_EXE) $(BUILD_DIR)/$(VISUAL_EXE)

# Build soccer_ball
$(BUILD_DIR)/$(SOCCER_EXE): $(SOCCER_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Build test_math
$(BUILD_DIR)/$(CUBE_EXE): $(CUBE_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Build test_visual
$(BUILD_DIR)/$(VISUAL_EXE): $(VISUAL_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# image target:
# 1. Run soccerball video executable and ffmpeg to create video (soccerball_video steps)
# 2. Convert clock face static image to png
# 3. Remove soccerball frames and video files
image: soccerball_video
	@if [ -f "$(PROJECT_DIR)/clock_face_static.pgm" ]; then \
		convert $(PROJECT_DIR)/clock_face_static.pgm $(PROJECT_DIR)/clock_face_static.png; \
	fi
	@rm -f $(PROJECT_DIR)/soccer_ball_frame_*.pgm $(PROJECT_DIR)/soccerball_rotation.mp4 2>/dev/null || true

# Generate cube video
cube_video: $(BUILD_DIR)/$(CUBE_EXE)
	cd $(BUILD_DIR) && ./$(CUBE_EXE)
	cd $(BUILD_DIR) && ffmpeg -framerate 24 -i frame_%04d.pgm -c:v libx264 -pix_fmt yuv420p cube_rotation.mp4
	@mv $(BUILD_DIR)/cube_rotation.mp4 $(PROJECT_DIR)/ 2>/dev/null || true

# Generate soccerball video
soccerball_video: $(BUILD_DIR)/$(SOCCER_EXE)
	cd $(BUILD_DIR) && ./$(SOCCER_EXE)
	cd $(BUILD_DIR) && ffmpeg -framerate 24 -i soccer_ball_frame_%03d.pgm -c:v libx264 -pix_fmt yuv420p soccerball_rotation.mp4
	@mv $(BUILD_DIR)/soccerball_rotation.mp4 $(PROJECT_DIR)/ 2>/dev/null || true

# Generate test_visual video
visual_video: $(BUILD_DIR)/$(VISUAL_EXE)
	cd $(BUILD_DIR) && ./$(VISUAL_EXE)
	cd $(BUILD_DIR) && ffmpeg -framerate 24 -i visual_%04d.pgm -c:v libx264 -pix_fmt yuv420p visual_rotation.mp4
	@mv $(BUILD_DIR)/visual_rotation.mp4 $(PROJECT_DIR)/ 2>/dev/null || true

# Run all videos
videos: cube_video soccerball_video visual_video

# Clean all generated files except build directory and demo.mp4
clean:
	@# Remove all PGM files from project directory
	@rm -f $(PROJECT_DIR)/*.pgm 2>/dev/null || true
	@# Remove all MP4 files from project directory except those we want to keep
	@for file in $(PROJECT_DIR)/*.mp4; do \
		if [ "$$file" != "$(PROJECT_DIR)/demo.mp4" ]; then \
			rm -f "$$file"; \
		fi; \
	done
	@# Clean build directory except demo.mp4 and the directory itself
	@if [ -d "$(BUILD_DIR)" ]; then \
		find $(BUILD_DIR) -type f ! -name 'demo.mp4' -delete; \
	fi