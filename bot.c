#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "ngawi_runtime.h"

/* Generated from main.ngawi */

void log_info(const char * message);
void log_success(const char * message);
void log_warn(const char * message);
void log_error(const char * message);
void log_debug(const char * message);
void log_start(const char * bot_name);
void log_plugin(int64_t count);
void log_message(int64_t chat, const char * sender, const char * message);
void log_command(int64_t chat, const char * command, const char * sender);
int main(void);
void cmd_ex(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args);
void cmd_menu(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args);
void cmd_help(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args);
void cmd_ping(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args);
void cmd_yts(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args);

static int64_t ng_plugin_count(void) { return 5; }
static const char *ng_plugin_name(int64_t index) {
  switch (index) {
    case 0: return "ex";
    case 1: return "menu";
    case 2: return "help";
    case 3: return "ping";
    case 4: return "yts";
    default: return "";
  }
}
static bool ng_plugin_exists(const char *name) {
  if (!name) return false;
  if (strcmp(name, "ex") == 0) return true;
  if (strcmp(name, "menu") == 0) return true;
  if (strcmp(name, "help") == 0) return true;
  if (strcmp(name, "ping") == 0) return true;
  if (strcmp(name, "yts") == 0) return true;
  return false;
}
static bool ng_dispatch_command(const char *token, int64_t chat, const char *command, const char *bot_name, const char *owner_name, const char *prefix, const char *args) {
  if (command && strcmp(command, "ex") == 0) {
    cmd_ex(token, chat, bot_name, owner_name, prefix, args);
    return true;
  }
  if (command && strcmp(command, "menu") == 0) {
    cmd_menu(token, chat, bot_name, owner_name, prefix, args);
    return true;
  }
  if (command && strcmp(command, "help") == 0) {
    cmd_help(token, chat, bot_name, owner_name, prefix, args);
    return true;
  }
  if (command && strcmp(command, "ping") == 0) {
    cmd_ping(token, chat, bot_name, owner_name, prefix, args);
    return true;
  }
  if (command && strcmp(command, "yts") == 0) {
    cmd_yts(token, chat, bot_name, owner_name, prefix, args);
    return true;
  }
  return false;
}

void log_info(const char * message)
{
  ng_print_string(ng_string_concat("[INFO] ", message));
  ng_print_string("\n");
}

void log_success(const char * message)
{
  ng_print_string(ng_string_concat("[SUCCESS] ", message));
  ng_print_string("\n");
}

void log_warn(const char * message)
{
  ng_print_string(ng_string_concat("[WARN] ", message));
  ng_print_string("\n");
}

void log_error(const char * message)
{
  ng_print_string(ng_string_concat("[ERROR] ", message));
  ng_print_string("\n");
}

void log_debug(const char * message)
{
  ng_print_string(ng_string_concat("[DEBUG] ", message));
  ng_print_string("\n");
}

void log_start(const char * bot_name)
{
  ng_print_string("");
  ng_print_string("\n");
  ng_print_string("—————————————————");
  ng_print_string("\n");
  ng_print_string(ng_string_concat(ng_string_concat(" ", bot_name), " - Telegram Bot"));
  ng_print_string("\n");
  ng_print_string("—————————————————");
  ng_print_string("\n");
  log_success("Bot berhasil dijalankan");
}

void log_plugin(int64_t count)
{
  ng_print_string("[PLUGIN] Loaded plugins:");
  ng_print_string("\n");
  ng_print_int(count);
  ng_print_string("\n");
}

void log_message(int64_t chat, const char * sender, const char * message)
{
  ng_print_string("[MESSAGE] Chat ID:");
  ng_print_string("\n");
  ng_print_int(chat);
  ng_print_string("\n");
  ng_print_string(ng_string_concat("[MESSAGE] Sender: ", sender));
  ng_print_string("\n");
  ng_print_string(ng_string_concat("[MESSAGE] Text: ", message));
  ng_print_string("\n");
}

void log_command(int64_t chat, const char * command, const char * sender)
{
  ng_print_string(ng_string_concat("[COMMAND] ", command));
  ng_print_string("\n");
  ng_print_string("[COMMAND] Chat ID:");
  ng_print_string("\n");
  ng_print_int(chat);
  ng_print_string("\n");
  ng_print_string(ng_string_concat("[COMMAND] Sender: ", sender));
  ng_print_string("\n");
}

int main(void)
{
  const char * config = ng_json_read("settings.json");
  const char * prefix = ng_json_get(config, "prefix");
  const char * token = ng_json_get(config, "bot.token");
  const char * bot_name = ng_json_get(config, "bot.name");
  const char * owner_name = ng_json_get(config, "owner.name");
  const char * api_url = ng_json_get(config, "api.baseUrl");
  const char * api_key = ng_json_get(config, "api.key");
  const char * api_header = ng_json_get(config, "api.header");
  ng_api_config(api_url, api_key, api_header);
  int64_t offset = 0;
  log_start(bot_name);
  log_plugin(ng_plugin_count());
  log_info("Connecting...");
  log_success("Bot is ready to receive messages");
  while (true)
  {
    const char * updates = ng_rusdi_get_update(token, offset);
    int64_t count = ng_telegram_count(updates);
    for (int64_t i = 0; (i < count); i = (i + 1))
    {
      const char * text = ng_telegram_text(updates, i);
      int64_t chat = ng_telegram_chat_id(updates, i);
      int64_t id = ng_telegram_update_id(updates, i);
      log_message(chat, "Telegram User", text);
      for (int64_t j = 0; (j < ng_plugin_count()); j = (j + 1))
      {
        const char * command = ng_plugin_name(j);
        const char * command_prefix = ng_string_concat(prefix, command);
        if ((ng_string_eq(text, command_prefix) || ng_string_starts_with(text, ng_string_concat(command_prefix, " "))))
        {
          const char * args = ng_string_trim(ng_string_slice(text, ng_string_len(command_prefix)));
          log_command(chat, command, "Telegram User");
          ng_dispatch_command(token, chat, command, bot_name, owner_name, prefix, args);
        }
      }
      offset = (id + 1);
    }
  }
  return 0;
}

void cmd_ex(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args)
{
  ng_rusdi_send(token, chat, "testing");
}

void cmd_menu(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args)
{
  const char * menu_text = ng_string_concat(ng_string_concat(ng_string_concat(ng_string_concat(ng_string_concat(ng_string_concat("*MENU BOT*\n\nNama Bot: ", bot_name), "\nOwner: "), owner_name), "\nPrefix: "), prefix), "\n\n*DAFTAR COMMAND*\n");
  for (int64_t i = 0; (i < ng_plugin_count()); i = (i + 1))
  {
    menu_text = ng_string_concat(ng_string_concat(ng_string_concat(menu_text, "\n- "), prefix), ng_plugin_name(i));
  }
  ng_rusdi_send(token, chat, menu_text);
}

void cmd_help(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args)
{
  const char * help_text = ng_string_concat(ng_string_concat(ng_string_concat(ng_string_concat("*BOT HELP*\n\nGunakan prefix `", prefix), "` sebelum nama command.\n\nKetik "), prefix), "menu untuk melihat semua command.");
  ng_rusdi_send(token, chat, help_text);
}

void cmd_ping(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args)
{
  const char * ping_text = ng_string_concat(ng_string_concat(ng_string_concat(ng_string_concat("*PONG!*\n\nStatus: Online\nBot: ", bot_name), "\nOwner: "), owner_name), "\nCatatan: Waktu respons real-time milidetik belum didukung oleh runtime Ngawi Lang saat ini.");
  ng_rusdi_send(token, chat, ping_text);
}

void cmd_yts(const char * token, int64_t chat, const char * bot_name, const char * owner_name, const char * prefix, const char * args)
{
  if (ng_string_eq(args, ""))
  {
    ng_rusdi_send(token, chat, ng_string_concat(ng_string_concat("Ex: ", prefix), "yts Billie Eilish"));
    return;
  }
  const char * query = ng_url_encode(ng_string_trim(args));
  const char * response = ng_api_get(ng_string_concat("/api/search/yts?q=", query));
  const char * data = ng_json_parse(response);
  if ((!ng_string_eq(ng_json_get(data, "status"), "true")))
  {
    ng_rusdi_send(token, chat, "Pencarian gagal");
    return;
  }
  const char * title1 = ng_json_get(data, "result[0].title");
  const char * title2 = ng_json_get(data, "result[1].title");
  const char * title3 = ng_json_get(data, "result[2].title");
  const char * title4 = ng_json_get(data, "result[3].title");
  const char * title5 = ng_json_get(data, "result[4].title");
  if (ng_string_eq(title1, ""))
  {
    ng_rusdi_send(token, chat, "Tidak ada hasil ditemukan.");
    return;
  }
  const char * message = "*[ SEARCH RESULTS ]*\n\n";
  message = ng_string_concat(ng_string_concat(ng_string_concat(message, "1. "), title1), "\n");
  message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Durasi: "), ng_json_get(data, "result[0].duration")), "\n");
  message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Views: "), ng_json_get(data, "result[0].views")), "\n");
  message = ng_string_concat(ng_string_concat(message, ng_json_get(data, "result[0].url")), "\n\n");
  if ((!ng_string_eq(title2, "")))
  {
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "2. "), title2), "\n");
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Durasi: "), ng_json_get(data, "result[1].duration")), "\n");
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Views: "), ng_json_get(data, "result[1].views")), "\n");
    message = ng_string_concat(ng_string_concat(message, ng_json_get(data, "result[1].url")), "\n\n");
  }
  if ((!ng_string_eq(title3, "")))
  {
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "3. "), title3), "\n");
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Durasi: "), ng_json_get(data, "result[2].duration")), "\n");
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Views: "), ng_json_get(data, "result[2].views")), "\n");
    message = ng_string_concat(ng_string_concat(message, ng_json_get(data, "result[2].url")), "\n\n");
  }
  if ((!ng_string_eq(title4, "")))
  {
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "4. "), title4), "\n");
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Durasi: "), ng_json_get(data, "result[3].duration")), "\n");
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Views: "), ng_json_get(data, "result[3].views")), "\n");
    message = ng_string_concat(ng_string_concat(message, ng_json_get(data, "result[3].url")), "\n\n");
  }
  if ((!ng_string_eq(title5, "")))
  {
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "5. "), title5), "\n");
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Durasi: "), ng_json_get(data, "result[4].duration")), "\n");
    message = ng_string_concat(ng_string_concat(ng_string_concat(message, "Views: "), ng_json_get(data, "result[4].views")), "\n");
    message = ng_string_concat(ng_string_concat(message, ng_json_get(data, "result[4].url")), "\n\n");
  }
  ng_rusdi_send(token, chat, message);
}

