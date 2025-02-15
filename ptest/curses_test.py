import subprocess

import curses

# result = subprocess.run(["ls", "-l"], capture_output=True, text=True)
# # print(type(result.stdout))
# lines = result.stdout.split('\n')
# for line in lines:
#         print(line)
# exit(0)

def subprocess_stdout_lines():
    result = subprocess.run(["ls", "-l"], capture_output=True, text=True)
    # print(type(result.stdout))
    return result.stdout.split('\n')

def curses_stdout_print(stdscr, line_list, lines_scrolled):
    """Print the test output"""
    height, width = stdscr.getmaxyx()

    max_render_height = height - 2
    line_list_size = len(line_list)
    rendered_row_count = min(max_render_height, line_list_size)
    
    # 
    list_index_start = lines_scrolled
    list_index_end = rendered_row_count + lines_scrolled

    
    row_index = 0
    for list_index in range(list_index_start, list_index_end):
        stdscr.addstr(row_index, 20, line_list[list_index])
        row_index = row_index + 1
   
    # SCROLL
    # top_overflow = len(line_list) - height
    # if top_overflow < 0:
    #     top_overflow = 0
    
    # for i, line in enumerate(line_list):
    #     if (i > top_overflow):
    #         stdscr.addstr(i-top_overflow, 30, line)  
    #         stdscr.refresh()

def curses_print_mouse_x_pos(stdscr):
    """Displays a message in the UI below the options."""
    height, width = stdscr.getmaxyx()

    mouse_pos = curses.getsyx()

    stdscr.addstr(10, 0, "X_POS = ?" + str(mouse_pos[0]))  # Show the message at the bottom
    stdscr.refresh()

def curses_highlight_region(stdscr):
    """highlight region"""
    height, width = stdscr.getmaxyx()
    row_start, col_start = 5, 1
    row_end, col_end = 10, 4

    # Highlight a region
    for row in range(row_start, row_end + 1):
        for col in range(col_start, col_end + 1):
            stdscr.addch(row, col, stdscr.inch(row, col) & 0xFF, curses.A_REVERSE)

def curses_ui_print(stdscr, message):
    """Displays a message in the UI below the options."""
    height, width = stdscr.getmaxyx()
    stdscr.addstr(height - 1, 0, message[:width - 1])  # Show the message at the bottom
    stdscr.refresh()

def main(stdscr):
    # Disable cursor and enable keypad input
    curses.curs_set(0)
    stdscr.keypad(True)

    # Options to toggle between
    options = ["subprocess", "pimage", "plib"]
    option_index = 0
    ui_message = "Press 'q' to quit."
    
    focusing_std_out = False
    stdout_scroll_index = 0
    """Always positive and less then stdoutput length"""
    stdout_string_list = []
    stdout_string_list = subprocess_stdout_lines()

    while True:
        stdscr.clear()

        # Display info calls
        curses_highlight_region(stdscr)
        curses_ui_print(stdscr, ui_message)
        curses_stdout_print(stdscr, stdout_string_list, stdout_scroll_index)
        curses_print_mouse_x_pos(stdscr)


        # TWO MODES
        if focusing_std_out: # no highlighting of options
            for idx, option in enumerate(options):
                    stdscr.addstr(idx, 0, f"  {option}")

            height, width = stdscr.getmaxyx()

            
            # Handle user input
            key = stdscr.getch()

            if key == curses.KEY_LEFT: # move back to options
                focusing_std_out = False
            elif key == curses.KEY_UP and stdout_scroll_index < len(stdout_string_list):
                stdout_scroll_index += 1
            elif key == curses.KEY_DOWN and stdout_scroll_index > 0:
                stdout_scroll_index -= 1
            elif key == ord("q"):
                break

        else: # Highlight current option index
            for idx, option in enumerate(options):
                if idx == option_index:
                    stdscr.addstr(idx, 0, f"> {option}", curses.A_REVERSE)
                else:
                    stdscr.addstr(idx, 0, f"  {option}")
            
            # Handle user input
            key = stdscr.getch()

            if key == curses.KEY_RIGHT: # move right
                focusing_std_out = True
            elif key == curses.KEY_UP and option_index > 0:
                option_index -= 1
            elif key == curses.KEY_DOWN and option_index < len(options) - 1:
                option_index += 1
            elif key == ord("\n"):
                if option_index == 0:
                    ui_message = "SUBPROCESS"
                    
                    # stdout_string_list.append("STDOUT STDOUT STDOUT")
                elif option_index == 1:
                    ui_message = "Run pimage!"
                elif option_index == 2:
                    ui_message = "Run plib!"

            elif key == ord("q"):
                break
        


if __name__ == "__main__":
    curses.wrapper(main)
