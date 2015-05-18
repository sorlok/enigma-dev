// Generated from Egm.g4 by ANTLR 4.5

 package hetu.seth.gen;
 
 import java.util.HashMap;
 import java.util.Deque;
 import java.util.ArrayDeque;

import org.antlr.v4.runtime.misc.NotNull;
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link EgmParser}.
 */
public interface EgmListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link EgmParser#rootRule}.
	 * @param ctx the parse tree
	 */
	void enterRootRule(EgmParser.RootRuleContext ctx);
	/**
	 * Exit a parse tree produced by {@link EgmParser#rootRule}.
	 * @param ctx the parse tree
	 */
	void exitRootRule(EgmParser.RootRuleContext ctx);
	/**
	 * Enter a parse tree produced by {@link EgmParser#block}.
	 * @param ctx the parse tree
	 */
	void enterBlock(EgmParser.BlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link EgmParser#block}.
	 * @param ctx the parse tree
	 */
	void exitBlock(EgmParser.BlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link EgmParser#blockBody}.
	 * @param ctx the parse tree
	 */
	void enterBlockBody(EgmParser.BlockBodyContext ctx);
	/**
	 * Exit a parse tree produced by {@link EgmParser#blockBody}.
	 * @param ctx the parse tree
	 */
	void exitBlockBody(EgmParser.BlockBodyContext ctx);
	/**
	 * Enter a parse tree produced by {@link EgmParser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(EgmParser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link EgmParser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(EgmParser.StatementContext ctx);
	/**
	 * Enter a parse tree produced by the {@code Assign}
	 * labeled alternative in {@link EgmParser#expression}.
	 * @param ctx the parse tree
	 */
	void enterAssign(EgmParser.AssignContext ctx);
	/**
	 * Exit a parse tree produced by the {@code Assign}
	 * labeled alternative in {@link EgmParser#expression}.
	 * @param ctx the parse tree
	 */
	void exitAssign(EgmParser.AssignContext ctx);
	/**
	 * Enter a parse tree produced by the {@code Prim}
	 * labeled alternative in {@link EgmParser#expression}.
	 * @param ctx the parse tree
	 */
	void enterPrim(EgmParser.PrimContext ctx);
	/**
	 * Exit a parse tree produced by the {@code Prim}
	 * labeled alternative in {@link EgmParser#expression}.
	 * @param ctx the parse tree
	 */
	void exitPrim(EgmParser.PrimContext ctx);
	/**
	 * Enter a parse tree produced by the {@code BinOp}
	 * labeled alternative in {@link EgmParser#expression}.
	 * @param ctx the parse tree
	 */
	void enterBinOp(EgmParser.BinOpContext ctx);
	/**
	 * Exit a parse tree produced by the {@code BinOp}
	 * labeled alternative in {@link EgmParser#expression}.
	 * @param ctx the parse tree
	 */
	void exitBinOp(EgmParser.BinOpContext ctx);
	/**
	 * Enter a parse tree produced by the {@code Func}
	 * labeled alternative in {@link EgmParser#expression}.
	 * @param ctx the parse tree
	 */
	void enterFunc(EgmParser.FuncContext ctx);
	/**
	 * Exit a parse tree produced by the {@code Func}
	 * labeled alternative in {@link EgmParser#expression}.
	 * @param ctx the parse tree
	 */
	void exitFunc(EgmParser.FuncContext ctx);
	/**
	 * Enter a parse tree produced by {@link EgmParser#paramList}.
	 * @param ctx the parse tree
	 */
	void enterParamList(EgmParser.ParamListContext ctx);
	/**
	 * Exit a parse tree produced by {@link EgmParser#paramList}.
	 * @param ctx the parse tree
	 */
	void exitParamList(EgmParser.ParamListContext ctx);
	/**
	 * Enter a parse tree produced by {@link EgmParser#primary}.
	 * @param ctx the parse tree
	 */
	void enterPrimary(EgmParser.PrimaryContext ctx);
	/**
	 * Exit a parse tree produced by {@link EgmParser#primary}.
	 * @param ctx the parse tree
	 */
	void exitPrimary(EgmParser.PrimaryContext ctx);
	/**
	 * Enter a parse tree produced by {@link EgmParser#literal}.
	 * @param ctx the parse tree
	 */
	void enterLiteral(EgmParser.LiteralContext ctx);
	/**
	 * Exit a parse tree produced by {@link EgmParser#literal}.
	 * @param ctx the parse tree
	 */
	void exitLiteral(EgmParser.LiteralContext ctx);
}