

set(SOURCES
  devices/device.cpp
  devices/device__square_wave.cpp
  devices/device__noise.cpp
  exprs/expr_element.cpp
  exprs/expr.cpp
  exprs/make_expr.cpp
  exprs/operate.cpp
  exprs/operation.cpp
  exprs/operand.cpp
  stmts/stmt.cpp
  stmts/stmt_list.cpp
  stmts/routine.cpp
  objects/reference.cpp
  objects/property.cpp
  objects/value.cpp
  scripts/script.cpp
  scripts/script_token.cpp
  scripts/script_token_string.cpp
  scripts/script_token_cursor.cpp
  execution_context.cpp
  shared_string.cpp
)




include_directories(./)
include_directories(../)
add_library("gbsnd" ${SOURCES})




