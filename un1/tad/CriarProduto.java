public class CriarProduto {
    public static void main(String[] args) {
        Produto p1 = new Produto();
        Produto p2 = new Produto();
        p1.adcProduto("arroz", 5);
        p1.remProduto(1);
        p1.altQuantidade(1, 3);
        p1.calcTotalEstoque(5, 33.2);

        p2.adcProduto("feijao", 2);
        p2.remProduto(2);
        p2.altQuantidade(2, 5);
        p2.calcTotalEstoque(4, 22.2);
    }
} 