

set(SOURCES
  streams/stream.cpp
  streams/stream__skip.cpp
  streams/stream__read_number.cpp
  exprs/expr_element.cpp
  exprs/expr.cpp
  exprs/make_expr.cpp
  exprs/read_operand.cpp
  exprs/operate.cpp
  exprs/operate__assignments.cpp
  exprs/operate__arithmetics.cpp
  exprs/operate__comparisons.cpp
  exprs/operate__bitwises.cpp
  exprs/operate__logicals.cpp
  exprs/operate__accesses.cpp
  exprs/operation.cpp
  exprs/operand.cpp
  stmts/stmt.cpp
  stmts/stmt_list.cpp
  stmts/routine.cpp
  values/value.cpp
  values/value__convert_to.cpp
  tables/variable.cpp
  tables/table.cpp
  tokens/token.cpp
  tokens/block.cpp
  tokens/cursor.cpp
  shared_string.cpp
  processes/process.cpp
  processes/process__call.cpp
  processes/process__step.cpp
  processes/process__load_file.cpp
)




include_directories(./)
include_directories(../)
add_library("gbscr" ${SOURCES})




