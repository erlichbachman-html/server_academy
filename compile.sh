#!/bin/bash

echo "Компиляция сервера..."

gcc server.c request.c response.c router.c -o server

echo "Сервер скомпилирован" 


echo "Компиляция клиента..."


gcc client.c -o client

echo "Клиент скомпилирован" 

