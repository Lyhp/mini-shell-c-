#include "parser.hpp"
#include <algorithm>
#include <cctype>

//Convierte una linea de texto en un pipeline de comandos

Pipeline Parser::parse_line(const std::string& line) {
    Pipeline pipeline;
    
    
    std::string trimmed = trim(line);
    if (trimmed.empty() || trimmed[0] == '#') {
        return pipeline;
    }
    
    // Paso 1: Dividir por pipes
    std::vector<std::string> parts = split_by_pipes(trimmed);
    
    // Paso 2: Detectar & al final
    bool bg = false;
    if (!parts.empty()) {
        std::string& last = parts.back();
        size_t amp_pos = last.find('&');
        if (amp_pos != std::string::npos) {
            bg = true;
            last = trim(last.substr(0, amp_pos));
        }
    }
    
    // Paso 3: Parsear cada comando
    for (const auto& part : parts) {
        std::vector<std::string> tokens = tokenize(part);
        if (!tokens.empty()) {
            Command cmd = parse_command(tokens);
            pipeline.commands.push_back(cmd);
        }
    }
    
    // Paso 4: Marcar background si aplica
    if (bg && !pipeline.commands.empty()) {
        pipeline.background = true;
        pipeline.commands.back().background = true;
    }
    
    return pipeline;

    // TOKENIZACIÓN (divide texto en palabras)

  std::vector<std::string> Parser::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_quotes = false;
    
    for (char c : text) {
       
        if (c == '"' || c == '\'') {
            in_quotes = !in_quotes;
            continue;
        }
        
        // Espacio separa tokens (excepto dentro de comillas)
        if (std::isspace(c) && !in_quotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            continue;
        }
        
        current += c;
    }
    
   
    if (!current.empty()) {
        tokens.push_back(current);
    }
    
    return tokens;
 }

  // DIVISIÓN POR PIPES

  std::vector<std::string> Parser::split_by_pipes(const std::string& line) {
    std::vector<std::string> parts;
    std::string current;
    bool in_quotes = false;
    
    for (char c : line) {
        if (c == '"' || c == '\'') {
            in_quotes = !in_quotes;
            current += c;
        } 
        else if (c == '|' && !in_quotes) {
            // Pipe encontrado fuera de comillas
            if (!current.empty()) {
                parts.push_back(trim(current));
                current.clear();
            }
        } 
        else {
            current += c;
        }
    }
    
    // Agregar última parte
    if (!current.empty()) {
        parts.push_back(trim(current));
    }
    
    return parts;
  }
}