public class Produto {

    String nome;
    Integer codigo;
    String descricao;
    Integer quantidade = 0;
    Number preco;

    public void adcProduto(String nomeProduto, Integer qtd) {
        nome = nomeProduto;
        quantidade += qtd;
        System.out.println("Produto adicionado: " + nome);
    }

    public void remProduto(Integer codigoProd) {
        if (codigo == codigoProd) {
            System.out.println("Produto com codigo: " + codigoProd + " foi removido");
        }
    }

    public void altQuantidade(Integer codigoProd, Integer qtd) {
        if (codigo == codigoProd) {
            quantidade = qtd;
            System.out.println("quantidade atualizada, agora e: " + quantidade);
        }
    }

    public String calcTotalEstoque(Integer qtd, Number precoUni) {
        Number total = 0;

        quantidade = qtd;
        preco = precoUni;

        total = quantidade.doubleValue() * preco.doubleValue();

        return "Total estoque: " + total;
    }
}