#include "token.h"

std::string Token::getKindName() const
{
    switch (kind)
    {
        default:
        case TokenKind::unknown:
            return "unknown";
        case TokenKind::s_comment:
            return "s_comment";
        case TokenKind::lm_comment:
            return "lm_comment";
        case TokenKind::rm_comment:
            return "rm_comment";
        case TokenKind::identifier:
            return "identifier";
        case TokenKind::numeric_constant:
            return "numeric_constant";
        case TokenKind::eof:
            return "eof";
        case TokenKind::l_paren:
            return "l_paren";
        case TokenKind::r_paren:
            return "r_paren";
        case TokenKind::l_brace:
            return "l_brace";
        case TokenKind::r_brace:
            return "r_brace";
        case TokenKind::star:
            return "tar";
        case TokenKind::plus:
            return "plus";
        case TokenKind::minus:
            return "minus";
        case TokenKind::minusequal:
            return "minuesequal";
        case TokenKind::exclaimequal:
            return "exclaimequal";
        case TokenKind::slash:
            return "lash";
        case TokenKind::less:
            return "less";
        case TokenKind::lessless:
            return "lessless";
        case TokenKind::lessequal:
            return "lessequal";
        case TokenKind::greater:
            return "greater";
        case TokenKind::greatergreater:
            return "greatergreater";
        case TokenKind::greaterequal:
            return "greaterequal";
        case TokenKind::semi:
            return "semi";
        case TokenKind::equal:
            return "equal";
        case TokenKind::equalequal:
            return "equalequal";
        case TokenKind::comma:
            return "comma";
        case TokenKind::kw_else:
            return "kw_else";
        case TokenKind::kw_if:
            return "kw_if";
        case TokenKind::kw_int:
            return "kw_int";
        case TokenKind::kw_return:
            return "kw_return";
        case TokenKind::kw_void:
            return "kw_void";
        case TokenKind::kw_while:
            return "kw_while";
    }
}
