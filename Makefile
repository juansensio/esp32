PORT := $(shell uv run mpremote connect list | awk '/303a:4001/ {print $$1; exit}')
DEVICE := id:123456

flash-id:
	uv run esptool flash-id

erase-flash:
	uv run esptool erase_flash

write-flash:
	uv run esptool --baud 460800 write_flash 0 ESP32_GENERIC_S3-20260824-v1.29.0.bin

list:
	uv run mpremote connect list

repl:
# 	@echo "Connecting to $(PORT)"
# 	uv run mpremote connect $(PORT) repl
# 	uv run mpremote connect $(DEVICE) repl
	uv run mpremote repl

run:
	uv run mpremote run $(or $(word 2,$(MAKECMDGOALS)),main.py)

%.py:
	@:

cp:
	uv run mpremote cp main.py :main.py

ls:
	uv run mpremote ls