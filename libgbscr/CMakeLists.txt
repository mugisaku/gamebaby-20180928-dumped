

set(SOURCES
  streams/stream.cpp
  streams/stream__skip.cpp
  streams/stream__read_number.cpp
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
  tokens/token.cpp
  tokens/token_string.cpp
  tokens/token_cursor.cpp
  execution_context.cpp
  shared_string.cpp
  process.cpp
)




include_directories(./)
include_directories(../)
add_library("gbscr" ${SOURCES})




