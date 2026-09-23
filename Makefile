PORT := $(shell uv run mpremote connect list | awk '/303a:4001/ {print $$1; exit}')
DEVICE := id:123456

list:
	uv run mpremote connect list

repl:
# 	@echo "Connecting to $(PORT)"
# 	uv run mpremote connect $(PORT) repl
# 	uv run mpremote connect $(DEVICE) repl
	uv run mpremote repl

run:
	uv run mpremote run main.py

cp:
	uv run mpremote cp main.py :main.py

ls:
	uv run mpremote ls