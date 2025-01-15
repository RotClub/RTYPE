# Format code

if command -v clang-format &> /dev/null; then
  find . -type f -name "*.cpp" | grep -e "client" -e "common" -e "server" | xargs -n1 "clang-format" "-i"
  find . -type f -name "*.hpp" | grep -e "client" -e "common" -e "server" | xargs -n1 "clang-format" "-i"
else
  echo "clang-format is not installed. Please install it to format the code."
fi
