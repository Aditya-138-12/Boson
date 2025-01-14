#include <gtk/gtk.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <array>
#include <set>
#include <sstream>
#include <algorithm>

// Welcome Message
const char* BOSON_ART = R"(
    ⠀⠀⠀⠀⠀⠀⣀⣤⣤⣶⣶⣶⣶⣤⣤⣀⡀⠀⠀⠀⠀⠀
       ⠀⣀⣴⣾⣿⡿⠛⠻⣿⣿⣿⣿⡿⠟⠛⢿⣿⣷⣦⣀⠀
    ⠀⣴⣿⣿⠋⠀⠀⠀⠀⠈⠉⠉⠁⠀⠀⠀⠀⠙⣿⣿⣿⣦⠀
    ⣼⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⣿⣧
    ⣿⣿⡇⠀⠀⢀⣤⣶⣶⣶⣶⣶⣶⣶⣦⣄⠀⠀⠀⢸⣿⣿⣿
    ⣿⣿⡇⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⢸⣿⣿⣿    		╔══════════════════════════════╗
    ⣿⣿⡇⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣿    		║     BOSON PARTICLE SHELL     ║
    ⣿⣿⡇⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣿    		║   Quantum Computing Terminal ║			
    ⣿⣿⡇⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⢸⣿⣿⣿    		╚══════════════════════════════╝
    ⣾⣿⣿⠀⠀⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⣿⣿⣿⣧
    ⠈⣿⣿⣧⠀⠀⠀⠉⠛⠿⢿⣿⠿⠛⠉⠀⠀⠀⣼⣿⣿⠁
    ⠀⠘⢿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⡿⠃⠀
    ⠀⠀⠀⠙⢿⣿⣿⣶⣤⣤⣤⣤⣤⣴⣶⣿⣿⡿⠋⠀⠀⠀
    ⠀⠀⠀⠀⠀⠙⠻⢿⣿⣿⣿⣿⡿⠟⠋⠀⠀⠀⠀                                                              
                     █████████████████████████████████████████
                ████            ████                          
              ██                    ███                       
            ██                █████████                 █     
          ██             ████           ████            █     
         ██           ███                 █  ██        ██     
        █           ██                     █   ██      ██     
       ██                                   █    ██    ██     
       █     █████ █████  ████   █     █    ██    ██   █      
      ██   ██      ██     █   █  ███   █    ██     ██  █      
      ██   █       █████  ████   █ ███ █     ██     ██ █      
      ██   ██      ██     █  █   █   ███     ██      ███      
      ██    ██████ ██████ █   ██ █     █    ████     ██       
       █                                    █  █     ██       
       ██      █                           ██  █     ██       
        ██     ██                         ██   ██    ██       
        ███    ██                        ██     █    █        
         █ ██   █                       ██      ██  ██        
         ██ ███ ██                    ██         █ ██         
          █    ██                  ████          █ █          
          ██      ████        ████ ██            ██           
           █        ██           ██            ██ █           
           ██         ██       ██            ██   ██          
                         ███  ██         ████      █          
                            ██ █████████           ██         
                          ██                        █         
                        ██                          █         
                       ██                           ██        
                     ██                              █        
                                                                                                    

Type 'help' to see available commands
    )";

// Set of available commands
const std::set<std::string> AVAILABLE_COMMANDS = {
    "ls", "pwd", "whoami", "date", "clear", "help"
};

const char* PROMPT = "user@boson:~$ ";

// Function to check if a command exists
bool command_exists(const std::string& cmd) {
    return AVAILABLE_COMMANDS.find(cmd) != AVAILABLE_COMMANDS.end();
}

// Function to get list of available commands
std::string get_available_commands() {
    std::stringstream ss;
    ss << "Available commands:\n";
    for (const auto& cmd : AVAILABLE_COMMANDS) {
        ss << "  " << cmd << "\n";
    }
    return ss.str();
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

// Function to handle built-in clear command
void clear_terminal(GtkTextBuffer* buffer) {
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gtk_text_buffer_delete(buffer, &start, &end);
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_insert(buffer, &start, PROMPT, -1);
}

// Function to handle built-in help command
std::string handle_help() {
    return get_available_commands();
}

// Enhanced command execution with built-in commands
std::string execute_command(const std::string& command) {
    // Trim the command
    std::string cmd = command;
    cmd.erase(0, cmd.find_first_not_of(" \t\n\r\f\v"));
    cmd.erase(cmd.find_last_not_of(" \t\n\r\f\v") + 1);

    // Handle empty command
    if (cmd.empty()) {
        return "";
    }

    // Handle built-in commands
    if (cmd == "help") {
        return handle_help();
    }

    // Check if command exists
    if (!command_exists(cmd)) {
        return "Command '" + cmd + "' not found.\n" + get_available_commands();
    }

    // Execute system command
    try {
        return exec(cmd.c_str());
    } catch (const std::exception& e) {
        return std::string("Error executing command: ") + e.what() + "\n";
    }
}

void handle_command(GtkTextBuffer* buffer) {
    GtkTextIter start, end, command_start;
    gtk_text_buffer_get_end_iter(buffer, &end);
    command_start = end;
    gtk_text_iter_backward_chars(&command_start, 1); // Move past the newline

    while (!gtk_text_iter_starts_line(&command_start)) {
        gtk_text_iter_backward_char(&command_start);
    }
    gtk_text_iter_forward_chars(&command_start, strlen(PROMPT));

    char* command_text = gtk_text_buffer_get_text(buffer, &command_start, &end, FALSE);
    g_strstrip(command_text);

    if (strlen(command_text) > 0) {
        std::string cmd(command_text);
        if (cmd == "clear") {
            clear_terminal(buffer);
        } else {
            std::string output = execute_command(cmd);
            gtk_text_buffer_get_end_iter(buffer, &end);
            gtk_text_buffer_insert(buffer, &end, "\n", -1);
            gtk_text_buffer_insert(buffer, &end, output.c_str(), -1);
            gtk_text_buffer_get_end_iter(buffer, &end);
            gtk_text_buffer_insert(buffer, &end, "\n", -1);
            gtk_text_buffer_insert(buffer, &end, PROMPT, -1);
        }
    } else {
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, "\n", -1);
        gtk_text_buffer_insert(buffer, &end, PROMPT, -1);
    }

    g_free(command_text);
}

gboolean on_key_press(GtkWidget* widget, GdkEventKey* event, gpointer user_data) {
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
    GtkTextIter insert, selection;
    gtk_text_buffer_get_iter_at_mark(buffer, &insert,
        gtk_text_buffer_get_insert(buffer));
    gtk_text_buffer_get_iter_at_mark(buffer, &selection,
        gtk_text_buffer_get_selection_bound(buffer));

    // Find the start of the current line
    GtkTextIter line_start = insert;
    gtk_text_iter_set_line_offset(&line_start, 0);
    
    // Calculate prompt position
    GtkTextIter prompt_end = line_start;
    gtk_text_iter_forward_chars(&prompt_end, strlen(PROMPT));

    // Don't allow editing prompt
    if (gtk_text_iter_compare(&insert, &line_start) < 0 ||
        gtk_text_iter_compare(&selection, &line_start) < 0) {
        return TRUE;
    }

    if (event->keyval == GDK_KEY_Return) {
        handle_command(buffer);
        return TRUE;
    }

    return FALSE;
}

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    // Create and configure the main window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Boson Terminal");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Add CSS class to window for styling
    gtk_style_context_add_class(gtk_widget_get_style_context(window), "terminal-window");

    GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    // Add CSS class to scrolled window
    gtk_style_context_add_class(gtk_widget_get_style_context(scrolled_window), "terminal-scroll");

    GtkWidget* text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), TRUE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    // Add CSS class to text view
    gtk_style_context_add_class(gtk_widget_get_style_context(text_view), "terminal-text");

    // Set text view properties
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 5);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 5);
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(text_view), 0);
    gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(text_view), 0);

    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    
    // Insert initial prompt and welcome message
    GtkTextIter iter;
    gtk_text_buffer_get_start_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, BOSON_ART, -1);
    gtk_text_buffer_insert(buffer, &iter, "\n", -1);  // Add extra newline for spacing
    gtk_text_buffer_insert(buffer, &iter, PROMPT, -1);

    // Connect key press event
    g_signal_connect(text_view, "key-press-event", G_CALLBACK(on_key_press), NULL);

    // Apply CSS styling
    GtkCssProvider* css_provider = gtk_css_provider_new();
    const char* css = R"(
        .terminal-window {
            background-color: black;
        }
        
        .terminal-scroll {
            background-color: black;
            border: none;
        }
        
        .terminal-scroll scrollbar {
            background-color: black;
            border: none;
        }
        
        .terminal-scroll scrollbar slider {
            background-color: #666666;
            border: none;
            border-radius: 0;
            min-width: 8px;
            min-height: 8px;
        }
        
        .terminal-scroll scrollbar slider:hover {
            background-color: #888888;
        }
        
        .terminal-text {
            background-color: black;
            color: white;
            font-family: "DejaVu Sans Mono", "Courier New", monospace;
            font-size: 12px;
            caret-color: white;
        }
        
        .terminal-text selection {
            background-color: white;
            color: black;
        }
        
        .terminal-text text {
            background-color: black;
            color: white;
            caret-color: white;
        }
        
        scrollbar {
            background-color: black;
            border: none;
        }
        
        window {
            background-color: black;
        }
    )";
    
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Show all widgets
    gtk_widget_show_all(window);
    
    // Start the GTK main loop
    gtk_main();

    return 0;
}
