idf_component_register(
  SRCS "main.c"
  INCLUDE_DIRS "."
  PRIV_REQUIRES bluetooth  # para linkar com o componente bluetooth acima
)
