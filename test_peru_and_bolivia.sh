make "$1".out

RED="\033[0;31m"
GREEN="\033[0;32m"
NC='\033[0m' # No Color
FILE="/tmp/peru_bolivia.tmp"
./bin/"$1".out >"$FILE"
ITER=$2

while [[ $ITER -gt 0 ]]; do
  while IFS= read -r line; do
    if [[ "$line" =~ ^((B[[:space:]])+|(P[[:space:]])+)$ ]]; then
      echo -e "${GREEN}OK: $line${NC}"
    else
      echo -e "${RED}ERROR: $line${NC}"
    fi
  done <"$FILE"
  echo -e "\n"
  ITER=$((ITER - 1))
done
