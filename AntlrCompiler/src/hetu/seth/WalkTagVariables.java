package hetu.seth;

import java.util.List;
import java.util.Map;

import hetu.seth.EgmCompiler.CodeGen;
import hetu.seth.gen.EgmBaseListener;
import hetu.seth.gen.EgmParser.AssignContext;
import hetu.seth.gen.EgmParser.BlockContext;
import hetu.seth.gen.EgmParser.ExpressionContext;
import hetu.seth.gen.EgmParser.FuncContext;
import hetu.seth.gen.EgmParser.LiteralContext;
import hetu.seth.gen.EgmParser.PrimContext;
import hetu.seth.gen.EgmParser.RootRuleContext;

class WalkTagVariables extends EgmBaseListener {
	public WalkTagVariables(CodeGen codeGen) {
		this.codeGen = codeGen;
	}
	
	public void exitBlockBody(hetu.seth.gen.EgmParser.BlockBodyContext ctx) {
		System.out.println("Block symbols:");
		System.out.println("  written {");
		for (Map.Entry<String, String> symb : ctx.writeSymbols.entrySet()) {
			System.out.println("    " + symb.getKey() + " (" + symb.getValue() + ")");
		}
		System.out.println("  }");
		System.out.println("  read {");
		for (Map.Entry<String, String> symb : ctx.readSymbols.entrySet()) {
			System.out.println("    " + symb.getKey() + " (" + symb.getValue() + ")");
		}
		System.out.println("  }");
	}
	
	public void enterLiteral(LiteralContext ctx) {
		System.out.println("Literal found: " + (ctx.IntegerLiteral()!=null?ctx.IntegerLiteral().getText():"") + " " + (ctx.FloatingPointLiteral()!=null?ctx.FloatingPointLiteral().getText():"") + " " + (ctx.StringLiteral()!=null?ctx.StringLiteral().getText():""));
	}
	
	public void enterPrim(PrimContext ctx) {
		//Skip literals; they're covered later.
		if (ctx.primary().variable!=null) {
		  System.out.println("Identifier found: " + ctx.primary().variable.getText());
		}
	}
	
	public void enterAssign(AssignContext ctx) {
		System.out.println("Assign found: " + ctx.variable.getText() + " := " + ctx.expression().getText());
	}
	public void exitAssign(AssignContext ctx) {
		System.out.println("EXIT assign");
	}
	
	public void enterFunc(FuncContext ctx) {
		System.out.println("Function: " + ctx.Identifier().getText());
	}

	private CodeGen codeGen;
	//private List<String> symbolTable = null;
}