/*
** EPITECH PROJECT, 2025
** Rtype [WSL: Ubuntu]
** File description:
** ArgumentManager
*/

#ifndef ARGUMENTMANAGER_HPP_
    #define ARGUMENTMANAGER_HPP_

    #include "spdlog/spdlog.h"
    #include "spdlog/sinks/stdout_color_sinks.h"
    #include "spdlog/sinks/daily_file_sink.h"

    class ArgumentManager {
        public:
            ArgumentManager(int ac, char **av);
            ~ArgumentManager();
            bool checkArguments();

        protected:
        private:
            int _ac;
            char **_av;
    };

#endif /* !ARGUMENTMANAGER_HPP_ */
